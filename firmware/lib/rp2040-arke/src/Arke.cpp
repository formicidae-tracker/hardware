#include "Arke.hpp"
#include "arke.h"

#include <hardware/irq.h>
#include <hardware/pio.h>
#include <hardware/platform_defs.h>
#include <hardware/regs/intctrl.h>
#include <hardware/watchdog.h>

#include <algorithm>
#include <cstdint>
#include <optional>

#include <utils/FlashStorage.hpp>
#include <utils/Log.hpp>
#include <utils/Queue.hpp>
#include <utils/Scheduler.hpp>

extern "C" {
#include <can2040.h>
}

struct ArkeNVData {
	uint8_t ID = 1;
};

class Arke {
public:
	void                   Init(ArkeConfig &&config);
	std::optional<int64_t> Work();

	void handleNetworkCommand(const struct can2040_msg &);
	void handleMessage(const struct can2040_msg &);
	void handleChangeID(const struct can2040_msg &);
	void handleHeartbeat(const struct can2040_msg &);

	void               transmitHeartbeat();
	struct can2040_msg buildHeartbeat() const;

	Queue<struct can2040_msg, 4, true> Messages;
	struct can2040                     CBus;

	ArkeNVData                     Data;
	uint8_t                        ClassMask;
	ArkeNodeClass_e                Class;
	std::optional<ArkeNodeVersion> NodeVersion;

	ArkeCallback Callback;

	std::optional<int64_t> HeartbeatPeriod;
};

static Arke arke;

void ArkeInit(ArkeConfig &&config) {
	arke.Init(std::move(config));
}

static void arke_can2040_irq_handler(void) {
	can2040_pio_irq_handler(&arke.CBus);
}

static void
arke_can2040_cb(struct can2040 *bus, uint32_t notify, struct can2040_msg *msg) {
	switch (notify) {
	case CAN2040_NOTIFY_TX:
		Debugf("[CAN] transmitted ID=%x Length=%d", msg->id, msg->dlc);
		break;
	case CAN2040_NOTIFY_ERROR:
		Warnf("[CAN]: error");
		break;
	case CAN2040_NOTIFY_RX:
		if (arke.Messages.TryEmplace(*msg) == false) {
			Errorf(
			    "[CAN]: must drop message ID=%x, Length=%d",
			    msg->id,
			    msg->dlc
			);
		}
		break;
	default:
		Errorf("[CAN]: unknown notify %x", notify);
	}
}

std::optional<int64_t> arkePong(absolute_time_t now) {
	return std::nullopt;
}

constexpr uint IRQS[4] = {
    PIO0_IRQ_0,
    PIO0_IRQ_1,
    PIO1_IRQ_0,
    PIO1_IRQ_1,
};

typedef FlashStorage<ArkeNVData, 0xfe, 1> ArkeNVStorage;

void Arke::Init(ArkeConfig &&config) {
	ArkeNVStorage::Load(Data);
	ClassMask   = config.ClassMask;
	Class       = config.Class;
	NodeVersion = config.NodeVersion;
	Callback    = std::move(config.Callback);

	Infof(
	    "[ARKE] Class:%02x ID:%x Mask:%02x",
	    config.Class,
	    Data.ID,
	    ClassMask
	);
	if (NodeVersion.has_value()) {
		Infof(
		    "[ARKE] Node Version %d.%d.%d.%d",
		    std::get<0>(NodeVersion.value()),
		    std::get<1>(NodeVersion.value()),
		    std::get<2>(NodeVersion.value()).value_or(0),
		    std::get<3>(NodeVersion.value()).value_or(0)
		);
	}

	Infof(
	    "[ARKE] PinTX: %d PinRX:%d, PIO: %d",
	    config.PinTX,
	    config.PinRX,
	    config.PIO
	);

	if (config.PIO > 1) {
		Fatalf("[ARKE]: Invalid PIO number %d, should be 0 or 1", config.PIO);
	}

	gpio_init(config.PinTX);
	gpio_set_dir(config.PinTX, true);

	can2040_setup(&CBus, config.PIO);

	can2040_callback_config(&CBus, arke_can2040_cb);

	for (uint i = 0; i < 2; ++i) {
		irq_set_exclusive_handler(
		    IRQS[2 * config.PIO + i],
		    arke_can2040_irq_handler
		);
		irq_set_priority(IRQS[2 * config.PIO + i], 1);
		irq_set_enabled(IRQS[2 * config.PIO + i], true);
	}

	Scheduler::Schedule(0, 0, [](absolute_time_t) { return arke.Work(); });

	can2040_start(&CBus, SYS_CLK_HZ, 250000, config.PinRX, config.PinTX);
}

#define arke_isNetworkCommand(a)  (((a)&0x600) == 0x000)
#define arke_isHeartBeat(a)       (((a)&0x600) == 0x600)
#define arke_isStandardMessage(a) (((a)&0x600) == 0x400)
#define arke_isPriorityMessage(a) (((a)&0x600) == 0x200)

std::optional<int64_t> Arke::Work() {
	struct can2040_msg msg;
	while (Messages.TryRemove(msg) == true) {
		Debugf(
		    "[ARKE]: received CAN message ID:0x%03x DLC:%d",
		    msg.id & 0x3ff,
		    msg.dlc
		);
		if (arke_isHeartBeat(msg.id)) {
			continue;
		}

		const uint8_t messageClass = (msg.id & 0x1ff) >> 3;
		const uint8_t messageID    = msg.id & 0x7;

		Debugf(
		    "[ARKE]: Type:%x Class:0x%02x ID:%d",
		    (msg.id & 0x600) >> 9,
		    messageClass,
		    messageID
		);

		if (arke_isNetworkCommand(msg.id) &&
		    (messageClass == uint8_t(Class) || messageClass == 0)) {
			Debugf("[ARKE] Network command for class:0x%02x", messageClass);
			handleNetworkCommand(msg);
		}

		if (arke_isStandardMessage(msg.id) &&
		    (messageClass & ClassMask) == uint8_t(Class) &&
		    (messageID == Data.ID || messageID == 0)) {
			this->Callback(ArkeEvent{
			    .Class = ArkeMessageClass_e(messageClass),
			    .RTR   = (msg.id & CAN2040_ID_RTR) != 0x00,
			    .Size  = std::min(uint8_t(8), uint8_t(msg.dlc)),
			    .Data  = msg.data,
			});
		}
	}

	return std::nullopt;
}

void Arke::handleNetworkCommand(const struct can2040_msg &msg) {
	switch (ArkeNetworkCommand_e(msg.id & 0x7)) {
	case ARKE_RESET_REQUEST:
		watchdog_enable(1, true);
		while (true) {
		};
		break;
	case ARKE_SYNCHRONISATION:
		Warnf("[Arke]: Synchronization Not implemented.");
		break;
	case ARKE_ID_CHANGE_REQUEST:
		handleChangeID(msg);
		break;
	case ARKE_ERROR_REPORT:
		Warnf("[ARKE]: Report error is not implemented.");
		break;
	case ARKE_HEARTBEAT_REQUEST:
		handleHeartbeat(msg);
		break;
	default:
		Errorf("[ARKE]: Unrecognized network command %x", msg.id & 0x7);
	}
}

void Arke::handleChangeID(const struct can2040_msg &msg) {
	if (msg.dlc != 2) {
		Errorf("[ARKE] Got change ID request, but dlc: %d (need 2)", msg.dlc);
	}
	if (msg.data[0] != Data.ID || msg.data[1] == 0x00 ||
	    msg.data[1] == msg.data[0]) {
		Warnf(
		    "[ARKE] Ignoring change ID request %x -> %x (Current ID:%x)",
		    msg.data[0],
		    msg.data[1],
		    Data.ID
		);
	}

	Data.ID = msg.data[1];
	ArkeNVStorage::Save(Data);
	Infof("[ARKE]: change ID to %x", Data.ID);
}

struct can2040_msg Arke::buildHeartbeat() const {
	uint32_t id = 0x600 | (uint32_t(Class) << 3) | Data.ID;
	if (NodeVersion.has_value() == false) {
		return {.id = id, .dlc = 0};
	}
	if (std::get<3>(NodeVersion.value()).has_value()) {
		return {
		    .id   = id,
		    .dlc  = 4,
		    .data = {
		        std::get<0>(NodeVersion.value()),
		        std::get<1>(NodeVersion.value()),
		        std::get<2>(NodeVersion.value()).value_or(0),
		        std::get<3>(NodeVersion.value()).value(),
		        0,
		        0,
		        0,
		        0}};
	}
	if (std::get<2>(NodeVersion.value()).has_value()) {
		return {
		    .id   = id,
		    .dlc  = 3,
		    .data = {
		        std::get<0>(NodeVersion.value()),
		        std::get<1>(NodeVersion.value()),
		        std::get<2>(NodeVersion.value()).value(),
		        0,
		        0,
		        0,
		        0,
		        0}};
	}

	return {
	    .id   = id,
	    .dlc  = 2,
	    .data = {
	        std::get<0>(NodeVersion.value()),
	        std::get<1>(NodeVersion.value()),
	        0,
	        0,
	        0,
	        0,
	        0,
	        0}};
}

void Arke::transmitHeartbeat() {
	if (can2040_check_transmit(&CBus) == 0) {
		Errorf("[ARKE]: could not schedule heartbeat transmit");
		return;
	}

	auto msg = buildHeartbeat();
	can2040_transmit(&CBus, &msg);
}

void Arke::handleHeartbeat(const struct can2040_msg &msg) {
	int64_t period = 0;
	if (msg.dlc == 2) {
		period = msg.data[1] + (uint16_t(msg.data[0]) << 8);
	} else if (msg.dlc != 0) {
		Errorf(
		    "[ARKE]: Invalid heartbeat message DLC %d (should be 0 or 2)",
		    msg.dlc
		);
	}
	if (period == 0) {
		Infof("[ARKE]: Single heartbeat response");
		HeartbeatPeriod = std::nullopt;
		transmitHeartbeat();
		return;
	}
	Infof("[ARKE]: Repetitive heartbeat, period = %lld ms", period);

	bool schedule   = HeartbeatPeriod.has_value() == false;
	HeartbeatPeriod = period * 1000;

	Scheduler::Schedule(
	    0,
	    HeartbeatPeriod.value(),
	    [](absolute_time_t) -> std::optional<int64_t> {
		    if (arke.HeartbeatPeriod.has_value() == false) {
			    return -1;
		    }
		    arke.transmitHeartbeat();
		    return arke.HeartbeatPeriod;
	    }
	);
}

namespace details {
void arkeSend(ArkeMessageClass_e cls, const uint8_t *data, size_t size) {
	if (can2040_check_transmit(&arke.CBus) == 0) {
		Errorf("[ARKE]: could not send message ID:%x DLC:%d", cls, size);
		return;
	}

	struct can2040_msg msg {
		.id = uint32_t(0x400) | (uint32_t(cls) << 3) |
		      (uint32_t(arke.Data.ID) & 0x007),
		.dlc = size,
	};

	memcpy(msg.data, data, size);
	can2040_transmit(&arke.CBus, &msg);
}
} // namespace details

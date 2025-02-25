#include "Arke.hpp"

#include "hardware/irq.h"
#include "hardware/pio.h"

#include <algorithm>
#include <cstdint>
#include <hardware/platform_defs.h>
#include <hardware/regs/intctrl.h>
#include <optional>

#include <utils/Log.hpp>

#include <utils/Queue.hpp>
#include <utils/Scheduler.hpp>

extern "C" {
#include <can2040.h>
}

class Arke {
public:
	void                   Init(ArkeConfig &&config);
	std::optional<int64_t> Work();

	Queue<struct can2040_msg, 4, true> Messages;
	struct can2040                     CBus;
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
		Infof("[CAN] transmitted ID=%x Length=%d", msg->id, msg->dlc);
		break;
	case CAN2040_NOTIFY_ERROR:
		Errorf("[CAN]: error");
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

void Arke::Init(ArkeConfig &&config) {
	printf("coucou %d\n", config.PIO);

	if (config.PIO > 1) {
		Fatalf("[ARKE]: Invalid PIO number %d, should be 0 or 1", config.PIO);
	}

	gpio_init(config.PinTX);
	gpio_set_dir(config.PinTX, true);

	can2040_setup(&CBus, config.PIO);

	can2040_callback_config(&CBus, arke_can2040_cb);

	printf("setting IRQ %d\n", config.PIO);
	for (uint i = 0; i < 2; ++i) {
		irq_set_exclusive_handler(
		    IRQS[2 * config.PIO + i],
		    arke_can2040_irq_handler
		);
		irq_set_priority(IRQS[2 * config.PIO + i], 1);
		irq_set_enabled(IRQS[2 * config.PIO + i], true);
	}

	printf("setting IRQ %d DONE\n", config.PIO);

	Scheduler::Schedule(0, 0, [](absolute_time_t) { return arke.Work(); });

	can2040_start(&CBus, SYS_CLK_HZ, 250000, config.PinRX, config.PinTX);
}

std::optional<int64_t> Arke::Work() {
	struct can2040_msg msg;
	while (Messages.TryRemove(msg) == true) {
		Infof(
		    "[CAN]: Got message ID=%x Length=%d, Data=%08x%08x",
		    msg.id,
		    msg.dlc,
		    msg.data32[0],
		    msg.data32[1]
		);
	}

	return std::nullopt;
}

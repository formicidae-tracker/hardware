#include "Arke.hpp"

#include "hardware/irq.h"

#include <Log.hpp>
#include <algorithm>
#include <cstdint>
#include <hardware/platform_defs.h>
#include <hardware/regs/intctrl.h>

#include "Log.hpp"
#include "Queue.hpp"
#include "Scheduler.hpp"

extern "C" {
#include "can2040.h"
}

struct Can2040Event {
	enum class EventType {
		TX,
		RX,
		ERROR,
	};
	EventType Type;
	uint32_t  IDT;
	uint8_t   Size;
	uint8_t   Data[8];

	Can2040Event()
	    : Type{EventType::ERROR}
	    , IDT{0xffffffff}
	    , Size{0} {}

	Can2040Event(uint32_t notify, const struct can2040_msg *msg)
	    : IDT{msg->id}
	    , Size{std::min(uint8_t(msg->dlc), uint8_t(8))} {
		switch (notify) {
		case CAN2040_NOTIFY_ERROR:
			Type = EventType::ERROR;
			IDT  = 0xffffffff;
			Size = 0;
			break;
		case CAN2040_NOTIFY_RX:
			Type = EventType::RX;
			break;
		case CAN2040_NOTIFY_TX:
			Type = EventType::TX;
			Size = 0;
			break;
		}

		std::copy(msg->data, msg->data + Size, Data);
	}
};

const char *EventTypeName(Can2040Event::EventType type) {
	switch (type) {
	case Can2040Event::EventType::TX:
		return "EventType::TX";
	case Can2040Event::EventType::RX:
		return "EventType::RX";
	case Can2040Event::EventType::ERROR:
		return "EventType::ERROR";
	default:
		return "EventType::<unknown>";
	}
}

class Arke {
public:
	struct can2040         d_bus;
	Queue<Can2040Event, 8> d_event;

	void Init(ArkeConfig &&config);
	void Work();
};

static Arke arke;

void ArkeInit(ArkeConfig &&config) {
	arke.Init(std::move(config));
}

static void arke_can2040_irq_handler(void) {
	can2040_pio_irq_handler(&arke.d_bus);
}

static void
arke_can2040_cb(struct can2040 *bus, uint32_t notify, struct can2040_msg *msg) {
	arke.d_event.EmplaceBlocking(notify, msg);
}

std::optional<int64_t> arkeWork(absolute_time_t now) {
	arke.Work();
	return std::nullopt;
}

void Arke::Init(ArkeConfig &&config) {
	can2040_setup(&d_bus, 0);

	can2040_callback_config(&d_bus, arke_can2040_cb);

	irq_set_exclusive_handler(PIO0_IRQ_0, arke_can2040_irq_handler);
	irq_set_priority(PIO0_IRQ_0, 1);
	irq_set_enabled(PIO0_IRQ_0, true);

	Scheduler::Schedule(0, 0, arkeWork);

	can2040_start(&d_bus, SYS_CLK_HZ, 250000, config.CanRX, config.CanTX);
}

void Arke::Work() {
	Can2040Event event;
	while (d_event.TryRemove(event) == true) {
		Infof(
		    "got a new event Type:%s IDT:0x%08x Size:%d",
		    EventTypeName(event.Type),
		    event.IDT,
		    event.Size
		);
	}
}

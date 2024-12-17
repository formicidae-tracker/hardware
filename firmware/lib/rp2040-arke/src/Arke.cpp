#include "Arke.hpp"

#include "hardware/irq.h"

#include <Log.hpp>
#include <cstdint>
#include <hardware/platform_defs.h>
#include <hardware/regs/intctrl.h>

#include "Queue.hpp"
#include "can2040.h"

class Arke {
public:
	struct can2040      d_bus;
	Queue<ArkeEvent, 8> d_event;

	void Init(ArkeConfig &&config);
};

static Arke arke;

void ArkeInit(ArkeConfig &&config) {
	arke.Init(std::move(config));
}

static void arke_can2040_irq_handler(void) {
	can2040_pio_irq_handler(&arke.d_bus);
}

static void
arke_can2040_cb(struct can2040 *bus, uint32_t notify, struct can2040_msg *) {
	switch (notify) {
	case CAN2040_NOTIFY_TX:
		return;
	case CAN2040_NOTIFY_ERROR:
		return;
	}
}

void Arke::Init(ArkeConfig &&config) {
	can2040_setup(&d_bus, 0);

	can2040_callback_config(&d_bus, arke_can2040_cb);

	irq_set_exclusive_handler(PIO0_IRQ_0, arke_can2040_irq_handler);
	irq_set_priority(PIO0_IRQ_0, 1);
	irq_set_enabled(PIO0_IRQ_0,true);

	can2040_start(&d_bus, SYS_CLK_HZ,250000,config.CanRX,config.CanTX);
}

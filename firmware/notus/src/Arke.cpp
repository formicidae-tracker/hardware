

#include <cstdint>
#include <hardware/irq.h>
#include <hardware/platform_defs.h>
#include <hardware/regs/intctrl.h>
#include <pico/types.h>
#include <pico/util/queue.h>

#include <cstdio>

extern "C" {
#include "can2040.h"
}

#include "Arke.hpp"
#include "utils/Scheduler.hpp"

static struct can2040 d_bus;
static queue_t        d_queue;

static void
ArkeCB(struct can2040 *cd, uint32_t notify, struct can2040_msg *msg) {
	msg->dlc = (msg->dlc & 0xf) | notify; // we muxes DLC and notify values.
	queue_try_add(&d_queue, &msg);        // push it to the queue, efficiently
}

void pio_irq_handler(void) {
	can2040_pio_irq_handler(&d_bus);
}

std::optional<int64_t> ArkeWork(absolute_time_t now) {
	struct can2040_msg *msg;
	while (queue_try_remove(&d_queue, &msg) == true) {
		printf("Got a new event\n");
	}
	return std::nullopt;
}

void ArkeInit() {
	printf("Initializing arke\n");
	queue_init(&d_queue, sizeof(struct can2040_msg), 8);

	can2040_setup(&d_bus, 0);
	can2040_callback_config(&d_bus, &ArkeCB);

	irq_set_exclusive_handler(PIO0_IRQ_0, pio_irq_handler);
	irq_set_priority(PIO0_IRQ_0, 1);
	irq_set_enabled(PIO0_IRQ_0, true);

	can2040_start(&d_bus, SYS_CLK_HZ, 250000, 13, 12);

	// Schedule our non-IRQ handler.
	Scheduler::Schedule(0, 0, &ArkeWork);
}

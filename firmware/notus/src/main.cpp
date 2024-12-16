#include "hardware/irq.h"
#include "pico/stdio.h"
#include "pico/time.h"

#include <cstdio>

extern "C" {
#include "can2040.h"
}

#include "utils/Scheduler.hpp"

static struct can2040 cbus;

static void
can2040_cb(struct can2040 *cd, uint32_t notify, struct can2040_msg *msg)
{
	printf("Got a new message\n");
}

static void PIOx_IRQHandler() {
	can2040_pio_irq_handler(&cbus);
}

std::optional<int64_t> print_uptime(absolute_time_t) {
	static int i = 0;
	printf("It is %d\n", ++i);
	return std::nullopt;
}

int main() {
	stdio_init_all();

	can2040_setup(&cbus, 0);
	can2040_callback_config(&cbus, can2040_cb);

	irq_set_exclusive_handler(PIO0_IRQ_0, PIOx_IRQHandler);
	irq_set_priority(PIO0_IRQ_0, 1);
	irq_set_enabled(PIO0_IRQ_0, true);

	can2040_start(&cbus, SYS_CLK_HZ, 250000, 13, 12);

	Scheduler::Schedule(100, 1000000, &print_uptime);
	while (true) {
		Scheduler::Work();
	}
}

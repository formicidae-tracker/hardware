#include "Arke.hpp"
#include "hardware/irq.h"
#include "pico/stdio.h"
#include "pico/time.h"

#include <cstdio>

extern "C" {
#include "can2040.h"
}

#include "Scheduler.hpp"

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

	ArkeInit();

	Scheduler::Schedule(100, 1000000, &print_uptime);
	while (true) {
		Scheduler::Work();
	}
}

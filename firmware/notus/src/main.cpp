#include "Arke.hpp"
#include "Scheduler.hpp"
#include "arke.h"
#include <hardware/gpio.h>
#include <pico/stdio.h>

#include <Log.hpp>

std::optional<int64_t> print_uptime(absolute_time_t) {
	static int i = 0;
	Infof("It is %d", i++);
	return std::nullopt;
}

int main() {
	stdio_init_all();

	Logger::InitLogsOnSecondCore();

	gpio_set_dir(14, true);
	gpio_set_dir(15, true);
	gpio_put(14, 1);
	gpio_put(15, 0);

	ArkeInit(ArkeConfig{
	    .CanRX     = 12,
	    .CanTX     = 13,
	    .Class     = ARKE_CELAENO,
	    .ClassMask = ARKE_CELAENO,
	    .Callback  = [](ArkeEvent) {},
	});

	Scheduler::Schedule(100, 1000000, &print_uptime);
	while (true) {
		Scheduler::Work();
	}
}

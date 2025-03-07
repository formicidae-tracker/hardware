#include <pico/stdlib.h>

#include <pico/types.h>
#include <utils/Log.hpp>

#include "LED.hpp"
#include "utils/Scheduler.hpp"

int main() {
	stdio_init_all();

	Logger::InitLogsOnSecondCore();
#ifndef NDEBUG
	Logger::Get().SetLevel(Logger::Level::DEBUG);
#endif
	LED green{11}, yellow{12};

	green.Set(255, 2 * 1000 * 1000);
	yellow.Blink(3);
	Scheduler::Schedule(1, 1000000, [](absolute_time_t) {
		Infof("Alive!");
		return std::nullopt;
	});
	for (;;) {
		Scheduler::Work();
	}
	return 0;
}

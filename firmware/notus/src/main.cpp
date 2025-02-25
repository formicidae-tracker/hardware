#include <hardware/gpio.h>
#include <optional>

#include <pico/stdlib.h>

#include <pico/types.h>
#include <utils/Log.hpp>
#include <utils/Scheduler.hpp>

#include <Arke.hpp>

constexpr static uint TCAN_TX       = 12;
constexpr static uint TCAN_RX       = 13;
constexpr static uint TCAN_SHUTDOWN = 14;
constexpr static uint TCAN_STANDBY  = 15;

int main() {
	stdio_init_all();

	Logger::InitLogsOnSecondCore();

	gpio_init(TCAN_SHUTDOWN);
	gpio_init(TCAN_STANDBY);
	gpio_set_dir(TCAN_SHUTDOWN, true);
	gpio_set_dir(TCAN_STANDBY, true);
	gpio_put(TCAN_SHUTDOWN, 0);
	gpio_put(TCAN_STANDBY, 0);

	ArkeInit(ArkeConfig{
	    .PinRX    = TCAN_RX,
	    .PinTX    = TCAN_TX,
	    .PIO      = 0,
	    .ID       = 1,
	    .Callback = [](const ArkeEvent &e) {},
	});

	while (true) {
		Scheduler::Work();
	}
}

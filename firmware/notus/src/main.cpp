#include <cstring>
#include <optional>

#include <hardware/gpio.h>
#include <pico/stdlib.h>
#include <pico/types.h>

#include <utils/Log.hpp>
#include <utils/Scheduler.hpp>

#include <Arke.hpp>

constexpr static uint TCAN_TX       = 12;
constexpr static uint TCAN_RX       = 13;
constexpr static uint TCAN_SHUTDOWN = 14;
constexpr static uint TCAN_STANDBY  = 15;

void onArkeEvent(const ArkeEvent &e) {
	switch (e.Class) {
	case ARKE_NOTUS_SET_POINT:
		if (e.RTR) {
			ArkeSend<uint8_t>(ARKE_NOTUS_SET_POINT, 0);
			break;
		}
		if (e.Size == 1) {
			Infof("[ARKE]: Setting power to %d", e.Data[0]);
			// TODO: set the power.
		} else {
			Warnf("[ARKE]: Unexpected DLC %d, (required: 1)", e.Size);
		}
		break;
	case ARKE_NOTUS_CONFIG:
		if (e.RTR) {
			ArkeSend<ArkeNotusConfig_t>(
			    ARKE_NOTUS_CONFIG,
			    ArkeNotusConfig_t{
			        .RampUpTimeMS   = 0,
			        .RampDownTimeMS = 0,
			        .MinOnTimeMS    = 0,
			    }
			);
			break;
		}
		if (e.Size != sizeof(ArkeNotusConfig_t)) {
			Warnf(
			    "[ARKE]: Unexpected DLC %d, (required: %d)",
			    e.Size,
			    sizeof(ArkeNotusConfig_t)
			);
		} else {
			ArkeNotusConfig_t config;
			memcpy((uint8_t *)(&config), e.Data, sizeof(ArkeNotusConfig_t));
			// TODO: set the config
		}

		break;
	default:
		Errorf(
		    "[ARKE]: Got event for 0x%x, but my mask is %x, check values",
		    e.Class,
		    ARKE_NOTUS
		);
	}
};

int main() {
	stdio_init_all();

	Logger::InitLogsOnSecondCore();
#ifndef NDEBUG
	Logger::Get().SetLevel(Logger::Level::DEBUG);
#endif

	gpio_init(TCAN_SHUTDOWN);
	gpio_init(TCAN_STANDBY);
	gpio_set_dir(TCAN_SHUTDOWN, true);
	gpio_set_dir(TCAN_STANDBY, true);
	gpio_put(TCAN_SHUTDOWN, 0);
	gpio_put(TCAN_STANDBY, 0);

	ArkeInit(ArkeConfig{
	    .PinRX       = TCAN_RX,
	    .PinTX       = TCAN_TX,
	    .PIO         = 0,
	    .Class       = ARKE_NOTUS,
	    .ClassMask   = ARKE_NOTUS,
	    .Callback    = onArkeEvent,
	    .NodeVersion = ArkeNodeVersion{0, 1, std::nullopt, std::nullopt},
	});

	while (true) {
		Scheduler::Work();
	}
}

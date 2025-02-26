#include "arke.h"
#include <cstring>
#include <hardware/platform_defs.h>
#include <optional>

#include <hardware/gpio.h>
#include <hardware/pwm.h>
#include <pico/stdlib.h>
#include <pico/types.h>

#include <utils/Log.hpp>
#include <utils/Scheduler.hpp>

#include <Arke.hpp>

constexpr static uint TCAN_TX       = 12;
constexpr static uint TCAN_RX       = 13;
constexpr static uint TCAN_SHUTDOWN = 14;
constexpr static uint TCAN_STANDBY  = 15;

class PWMPin {
public:
	PWMPin(uint pin, uint max = 255, uint freq_hz = 200)
	    : d_slice{pwm_gpio_to_slice_num(pin)}
	    , d_channel{pwm_gpio_to_channel(pin)} {

		auto config = pwm_get_default_config();
		pwm_config_set_clkdiv(
		    &config,
		    float(SYS_CLK_HZ) / float(max * freq_hz)
		);
		pwm_config_set_wrap(&config, max - 1);
		gpio_set_function(pin, GPIO_FUNC_PWM);
		Set(0);
		pwm_init(d_slice, &config, true);
	}

	void Set(uint value) {
		pwm_set_chan_level(d_slice, d_channel, 0);
	}

private:
	uint d_slice, d_channel;
};

class Heater {
public:
	Heater(uint fanPin, uint heatPin)
	    : d_heat{heatPin}
	    , d_fan{fanPin}
	    , d_config{ArkeNotusConfig_t{
	          .RampDownTimeMS = 2000,
	          .MinFan         = 50,
	          .MaxHeat        = 200,
	      }}
	    , d_state{State::IDLE} {}

private:
	enum class State {
		IDLE,
		ON,
		RAMP_DOWN,
	};

	PWMPin            d_heat, d_fan;
	ArkeNotusConfig_t d_config;
};

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
		Errorf("[ARKE]: Unrecognized class message %x", e.Class);
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

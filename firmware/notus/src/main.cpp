#include "arke.h"
#include <atomic>
#include <cstring>
#include <hardware/platform_defs.h>
#include <optional>

#include <hardware/gpio.h>
#include <hardware/pwm.h>
#include <pico/stdlib.h>
#include <pico/time.h>
#include <pico/types.h>

#include <utils/FlashStorage.hpp>
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
	    , d_fan{fanPin} {
		NVStorage::Load(d_config);
	}

	std::optional<int64_t> work(absolute_time_t now) {
		switch (d_state) {
		case State::IDLE:
		case State::ON:
			return std::nullopt;
		case State::RAMP_DOWN:
			if (absolute_time_diff_us(d_rampDownStart, now) >
			    (d_config.RampDownTimeMS * 1000)) {
				d_fan.Set(0);
				d_heat.Set(0);
				d_state = State::IDLE;
			}
			return std::nullopt;
		}
	}

	uint8_t Level() const {
		return d_level;
	}

	void Set(uint8_t level) {
		if (level == d_level) {
			return;
		}
		d_level = level;

		if (level == 0) {
			if (d_state == State::ON) {
				// switch to ramp down, no heat, min fan for
				// d_config.RampDownTimeMS
				d_rampDownStart = get_absolute_time();
				d_fan.Set(d_config.MinFan);
				d_heat.Set(0);
				d_state = State::RAMP_DOWN;
			}
			return;
		}
		// compute the actual level of fan and heat.
		uint8_t fanLevel = std::max(
		    0,
		    std::min(
		        255,
		        int(level) * (255 - d_config.MinFan) / 255 + d_config.MinFan
		    )
		);
		d_fan.Set(fanLevel);
		d_heat.Set(level);
		d_state = State::ON;
	}

	const ArkeNotusConfig_t &Config() const {
		return d_config;
	}

	void SetConfig(const ArkeNotusConfig_t &config) {
		d_config = config;
		NVStorage::Save(config);
	}

private:
	typedef FlashStorage<ArkeNotusConfig_t, 1, 1> NVStorage;
	enum class State {
		IDLE,
		ON,
		RAMP_DOWN,
	};

	PWMPin            d_heat, d_fan;
	ArkeNotusConfig_t d_config = ArkeNotusConfig_t{
	    .RampDownTimeMS = 2500,
	    .MinFan         = 50,
	    .MaxHeat        = 255,
	};
	State           d_state = State::IDLE;
	uint8_t         d_level = 0;
	absolute_time_t d_rampDownStart = 0;
};

static Heater heater{22, 23};

void onArkeEvent(const ArkeEvent &e) {
	switch (e.Class) {
	case ARKE_NOTUS_SET_POINT:
		if (e.RTR) {
			ArkeSend<uint8_t>(ARKE_NOTUS_SET_POINT, heater.Level());
			break;
		}
		if (e.Size == 1) {
			Infof("[ARKE]: Setting power to %d", e.Data[0]);
			heater.Set(e.Data[0]);
		} else {
			Warnf("[ARKE]: Unexpected DLC %d, (required: 1)", e.Size);
		}
		break;
	case ARKE_NOTUS_CONFIG:
		if (e.RTR) {
			ArkeSend<ArkeNotusConfig_t>(ARKE_NOTUS_CONFIG, heater.Config());
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
			heater.SetConfig(config);
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

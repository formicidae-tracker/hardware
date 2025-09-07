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
	static constexpr uint shift = 4;

public:
	PWMPin(uint pin, uint freq_hz = 240)
	    : d_slice{pwm_gpio_to_slice_num(pin)}
	    , d_channel{pwm_gpio_to_channel(pin)} {
		static constexpr uint max    = 256 << shift;
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

	void Set(uint8_t value) {
		if (value == 0) {
			pwm_set_chan_level(d_slice, d_channel, 0);
		} else {
			pwm_set_chan_level(
			    d_slice,
			    d_channel,
			    (uint(value) << shift) + (1 << shift) - 1
			);
		}
	}

private:
	uint d_slice, d_channel;
};

class Heater {
public:
	Heater(uint fanPin, uint heatPin)
	    : d_heat{heatPin}
	    , d_fanPin{fanPin} {
		gpio_init(d_fanPin);
		gpio_set_dir(d_fanPin, true);
		gpio_put(d_fanPin, false);

		NVStorage::Load(d_config);
		Scheduler::Schedule(100, 1000, [this](absolute_time_t now) {
			return work(now);
		});
	}

	std::optional<int64_t> work(absolute_time_t now) {
		switch (d_state) {
		case State::IDLE:
		case State::ON:
			return std::nullopt;
		case State::RAMP_DOWN:
			if (absolute_time_diff_us(d_rampDownStart, now) >
			    (d_config.RampDownTimeMS * 1000)) {
				Infof("Ramp down done");
				gpio_put(d_fanPin, true);
				d_heat.Set(0);
				d_state = State::IDLE;
			}
			return std::nullopt;
		default:
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
				Infof("Ramping down...");
				gpio_put(d_fanPin, true);
				d_heat.Set(0);
				d_state = State::RAMP_DOWN;
			}
			return;
		}
		gpio_put(d_fanPin, true);
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

	PWMPin            d_heat;
	uint              d_fanPin;
	ArkeNotusConfig_t d_config = ArkeNotusConfig_t{
	    .RampDownTimeMS = 2500,
	    .MinFan         = 100,
	    .MaxHeat        = 255,
	};
	State           d_state = State::IDLE;
	uint8_t         d_level = 0;
	absolute_time_t d_rampDownStart = 0;
};

static std::unique_ptr<Heater> heater;

void onArkeEvent(const ArkeEvent &e) {
	switch (e.Class) {
	case ARKE_NOTUS_SET_POINT:
		if (e.RTR) {
			ArkeSend<uint8_t>(ARKE_NOTUS_SET_POINT, heater->Level());
			break;
		}
		if (e.Size == 1) {
			Infof("[ARKE]: Setting power to %d", e.Data[0]);
			heater->Set(e.Data[0]);
		} else {
			Warnf("[ARKE]: Unexpected DLC %d, (required: 1)", e.Size);
		}
		break;
	case ARKE_NOTUS_CONFIG:
		if (e.RTR) {
			ArkeSend<ArkeNotusConfig_t>(ARKE_NOTUS_CONFIG, heater->Config());
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
			heater->SetConfig(config);
		}

		break;
	default:
		// Errorf("[ARKE]: Unrecognized class message %x", e.Class);
	}
};

int main() {
	stdio_init_all();

	Logger::InitLogsOnSecondCore();
#ifndef NDEBUG
	Logger::Get().SetLevel(Logger::Level::DEBUG);
#endif

	heater = std::make_unique<Heater>(22, 23);

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

#include <functional>
#include <optional>

#include <hardware/gpio.h>
#include <hardware/uart.h>
#include <pico/stdlib.h>
#include <pico/time.h>
#include <pico/types.h>

#include <utils/FlashStorage.hpp>
#include <utils/Log.hpp>
#include <utils/Scheduler.hpp>

#include "Arke.hpp"

#include "LED.hpp"
#include "arke.h"
#include "config_helios.hpp"

template <size_t N> class SlaveCommunicationMultiplexer {
public:
	constexpr static int64_t MIN_PERIOD       = 1e6 / 30.5;
	constexpr static int64_t MAX_PULSE        = 4000;
	constexpr static int     BAUDRATE         = 20000;
	constexpr static uint8_t TASKLET_PRIORITY = 10;

	template <typename... Args>
	SlaveCommunicationMultiplexer(Args... args)
	    : d_pins{args...} {
		for (size_t i = 0; i < N; ++i) {
			const auto &p = d_pins[0];
			gpio_set_dir(p, GPIO_OUT);
			gpio_put(p, i == 0 ? true : false);
			gpio_set_function(p, GPIO_FUNC_SIO);
		}

		uart_init(uart1, BAUDRATE);
	}

	void StartPulse() {
		if (d_state != State::IDLE || d_inhibited == true) {
			return;
		}

		d_state = State::PULSE;
		for (size_t i = 0; i < N; ++i) {
			gpio_put(d_pins[i], i == 0 ? false : true);
		}

		Scheduler::Schedule(
		    TASKLET_PRIORITY,
		    -MAX_PULSE,
		    [this](absolute_time_t) {
			    EndPulse();
			    return -1;
		    }
		);
	}

	void EndPulse() {
		if (d_state != State::PULSE) {
			return;
		}
		for (size_t i = 0; i < N; ++i) {
			gpio_put(d_pins[i], i == 0 ? true : false);
		}

		d_state = State::PULSE_BREAK;

		// schedule a send in 1ms
		Scheduler::Schedule(0, -1000, [this](absolute_time_t) {
			sendValue();
			return -1;
		});

		// schedule a rearm after min period.
		Scheduler::Schedule(
		    TASKLET_PRIORITY,
		    MAX_PULSE - MIN_PERIOD,
		    [this](absolute_time_t) {
			    rearm();
			    return -1;
		    }
		);
	}

	void SetInhibited(bool inhibit) {
		if (d_inhibited == inhibit) {
			return;
		}
		if (inhibit == true) {
			EndPulse();
		}

		d_inhibited = inhibit;
	}

	void SetPoint(ArkeHeliosSetPoint_t value) {
		d_nextValue = value;
		if (d_state != State::IDLE) {
			return;
		}
		d_state = State::PULSE_BREAK;
		sendValue();
		Scheduler::Schedule(TASKLET_PRIORITY, -1000, [this](absolute_time_t) {
			rearm();
			return -1;
		});
	}

private:
	enum class State {
		IDLE,        // Serial allowed.
		PULSE,       // Sending pulse // Serial allowed
		PULSE_BREAK, // Break, Serial is now.
	};

	void sendValue() {
		if (d_nextValue.has_value() == false) {
			return;
		}
		gpio_set_function(d_pins[0], UART_FUNCSEL_NUM(1, d_pins[0]));
		d_buffer[0] = 0xaa;
		d_buffer[1] = d_nextValue.value().Visible;
		d_buffer[2] = d_nextValue.value().UV;
		d_buffer[3] = 0x00;
		uart_puts(uart1, d_buffer);
		d_nextValue = std::nullopt;
	}

	void rearm() {
		gpio_set_function(d_pins[0], UART_FUNCSEL_NUM(1, d_pins[0]));
		d_state = State::IDLE;
	}

	bool d_inhibited = false;

	State d_state = State::IDLE;

	uint d_txPin, outPin;

	std::optional<ArkeHeliosSetPoint_t> d_nextValue;
	char                                d_buffer[4];

	std::array<uint, N> d_pins;
};

class Helios {
public:
	typedef FlashStorage<ArkeHeliosSetPoint_t, 1> NVStorage;

	struct Args {
		uint SyncOutPin, InIsolatedPin, InTTLPin, TxPin, StrobePin, InhibitPin;
	};

	constexpr static int64_t VISIBLE_UPDATE_PERIOD_US = 200 * 1000;
	constexpr static int64_t SYNC_SIGNAL_US           = 1000;

	Helios(Args &&args)
	    : d_slaves{args.TxPin, args.StrobePin}
	    , d_pins{std::move(args)}
	    , d_lastVisibleUpdate(get_absolute_time() - VISIBLE_UPDATE_PERIOD_US) {
		gpio_init(d_pins.SyncOutPin);
		gpio_set_dir(d_pins.SyncOutPin, GPIO_OUT);
		gpio_put(d_pins.SyncOutPin, 0);

		gpio_init(d_pins.InIsolatedPin);
		gpio_init(d_pins.InTTLPin);
		gpio_init(d_pins.InTTLPin);

		NVStorage::Load(d_setPoint);
	}

	const ArkeHeliosSetPoint_t &Visible() {
		return d_setPoint;
	}

	void SetVisible(const ArkeHeliosSetPoint_t &value) {
		Infof(
		    "[Helios]: Setting visible point to Visible:%d UV:%d",
		    value.Visible,
		    value.UV
		);
		d_slaves.SetPoint(value);
		d_setPoint          = value;
		d_lastVisibleUpdate = get_absolute_time();
		NVStorage::Save(d_setPoint);
	}

	uint16_t VisiblePulsation_us() {
		return d_visiblePulsationPeriod_us;
	}

	ArkeHeliosTriggerConfig_t Trigger() {
		return ArkeHeliosTriggerConfig_t{
		    .Period_hecto_us = uint16_t(d_pulsePeriod_us / 100),
		    .Pulse_us        = uint16_t(d_pulseWidth_us),
		};
	}

	void SetTrigger(const ArkeHeliosTriggerConfig_t &config) {
		bool triggerNow  = d_pulsePeriod_us <= 0;
		d_pulsePeriod_us = config.Period_hecto_us * 100;
		d_pulseWidth_us  = config.Pulse_us;
		if (triggerNow) {
			d_lastPulse = get_absolute_time() - d_pulsePeriod_us;
		}
	}

private:
	void work() {
		d_slaves.SetInhibited(gpio_get(d_pins.InhibitPin));

		bool hasTrigger =
		    gpio_get(d_pins.InIsolatedPin) || gpio_get(d_pins.InTTLPin);

		auto now = get_absolute_time();

		if (d_pulsePeriod_us > 0) {
			generatePulse(now);
		} else {
			if (d_onPulse == false && hasTrigger) {
				d_onPulse = true;
				d_slaves.StartPulse();
				Debugf("[Helios]: External Pulse Start");
			}
			if (d_onPulse == true && hasTrigger == false) {
				d_onPulse = false;
				d_slaves.EndPulse();
				Debugf("[Helios]: External Pulse End");
			}
		}

		if (absolute_time_diff_us(d_lastVisibleUpdate, now) >
		    VISIBLE_UPDATE_PERIOD_US) {
			Debugf("[Helios]: Visible set point update");
			d_slaves.SetPoint(d_setPoint);
			d_lastVisibleUpdate = now;
		}
	}

	void generatePulse(absolute_time_t now) {
		auto ellapsed = absolute_time_diff_us(d_lastPulse, now);
		if (d_onPulse && ellapsed > d_pulseWidth_us) {
			d_slaves.EndPulse();
			Debugf("[Helios]: Pulse End");
			d_onPulse = false;
		}
		if (d_onSync && ellapsed > SYNC_SIGNAL_US) {
			gpio_put(d_pins.SyncOutPin, 0);
			Debugf("[Helios]: Sync End");
			d_onSync = false;
		}

		if (d_onPulse == false && d_onSync == false &&
		    ellapsed > d_pulsePeriod_us) {
			d_slaves.StartPulse();
			d_onPulse = true;
			d_onSync  = true;
			gpio_put(d_pins.SyncOutPin, 1);
			d_lastPulse += d_pulsePeriod_us;
			Debugf("[Helios]: Pulse and Sync Start");
		}
	}

	    SlaveCommunicationMultiplexer<2> d_slaves;
		Args                             d_pins;

		ArkeHeliosSetPoint_t d_setPoint = {.Visible = 0, .UV = 0};

		uint16_t        d_visiblePulsationPeriod_us = 0;
		int64_t         d_pulsePeriod_us            = 0;
		int64_t         d_pulseWidth_us             = 0;
		absolute_time_t d_lastVisibleUpdate, d_lastPulse;
		bool            d_onPulse = false;
		bool            d_onSync  = false;
	};

static Helios helios{Helios::Args{
    .SyncOutPin    = 25,
    .InIsolatedPin = 19,
    .InTTLPin      = 23,
    .TxPin         = 8,
    .StrobePin     = 24,
    .InhibitPin    = 20,
}};

constexpr static uint TCAN_SHUTDOWN = 26;
constexpr static uint TCAN_STANDBY  = 27;
constexpr static uint TCAN_RX       = 41;
constexpr static uint TCAN_TX       = 40;

void onArkeEvent(const ArkeEvent &e) {
	switch (e.Class) {
	case ARKE_HELIOS_SET_POINT:
		if (e.RTR) {
			ArkeSend<ArkeHeliosSetPoint_t>(
			    ARKE_HELIOS_SET_POINT,
			    helios.Visible()
			);
			break;
		}
		if (e.Size == sizeof(ArkeHeliosSetPoint_t)) {
			helios.SetVisible(e.as<ArkeHeliosSetPoint_t>());
		} else {
			Warnf(
			    "[ARKE]: Unexpected DLC %d, (required %d)",
			    e.Size,
			    sizeof(ArkeHeliosSetPoint_t)
			);
		}

		break;
	case ARKE_HELIOS_PULSE_MODE:
		Warnf("[ARKE]: Not implemented yet");
		break;
	case ARKE_HELIOS_TRIIGER_MODE:
		if (e.RTR) {
			ArkeSend<ArkeHeliosTriggerConfig_t>(
			    ARKE_HELIOS_TRIIGER_MODE,
			    helios.Trigger()
			);
			break;
		}

		if (e.Size == sizeof(ArkeHeliosTriggerConfig_t)) {
			helios.SetTrigger(e.as<ArkeHeliosTriggerConfig_t>());
		} else {
			Warnf(
			    "[ARKE]: Unexpected DLC %d, (required: %d)",
			    e.Size,
			    sizeof(ArkeHeliosTriggerConfig_t)
			);
		}
		break;
	default:
		Warnf("[ARKE]: Unexpected message 0x%03X DLC:%d", e.Class, e.Size);
	}
}

void init() {
	stdio_init_all();

	Logger::InitLogsOnSecondCore();
#ifndef NDEBUG
	Logger::Get().SetLevel(Logger::Level::DEBUG);
#endif

	gpio_init(TCAN_SHUTDOWN);
	gpio_init(TCAN_STANDBY);
	gpio_set_dir(TCAN_SHUTDOWN, GPIO_OUT);
	gpio_set_dir(TCAN_STANDBY, GPIO_OUT);
	gpio_put(TCAN_SHUTDOWN, 0);
	gpio_put(TCAN_STANDBY, 0);

	ArkeInit(ArkeConfig{
	    .PinRX     = TCAN_RX,
	    .PinTX     = TCAN_TX,
	    .PIO       = 0,
	    .Class     = ARKE_HELIOS,
	    .ClassMask = ARKE_HELIOS,
	    .Callback  = onArkeEvent,
	    .NodeVersion =
	        ArkeNodeVersion{
	            HELIOS_VERSION_MAJOR,
	            HELIOS_VERSION_MINOR,
	            HELIOS_VERSION_PATCH,
	            std::nullopt,
	        },
	});
};

int main() {
	init();
	LED green{11}, yellow{12};

#ifndef NDEBUG
	Scheduler::Schedule(1, 1000000, [](absolute_time_t) {
		Infof("Alive!");
		return std::nullopt;
	});
#endif

	for (;;) {
		Scheduler::Work();

	}
	return 0;
}

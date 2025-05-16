#include <cstdint>
#include <functional>

extern "C" {
#include <hardware/sync.h>
#include <hardware/gpio.h>
#include <hardware/uart.h>
#include <pico/lock_core.h>
#include <pico/stdlib.h>
#include <pico/time.h>
#include <pico/types.h>
#include <pico/multicore.h>
}

#include <memory>
#include <optional>


#include <utils/Duration.hpp>
#include <utils/FlashStorage.hpp>
#include <utils/Log.hpp>
#include <utils/Scheduler.hpp>

#include "Arke.hpp"

#include "LED.hpp"
#include "arke.h"
#include "config_helios.hpp"


// This whole code, overly complicated, isn't working.. There are interupt,
// there are life, it makes jitters a nightmare... We need to use
// peripheral!!!!!
//
// 1. Drop support for Visible, Trigger input and inhibit and sync.
//
//
// 2. We use a PIO pwm to configure the pulse length. pulse period is fixed once
// started. We do not sync !!!
// //
// //
// // 2. If UART for visible is needed, we should multiplex in software the output
// // using a PIO to detect an end of pulse and check when it is safe to multiplex
// // (no need now) by switching the pin to UART.
// //

// //
//
// 4. check for inhibit and trigger input support via PIO or interrupt (maybe
// interrupt from another core is better??)
//

template <size_t N> class SlaveCommunicationMultiplexer {
public:
	constexpr static int64_t MIN_PERIOD_us = 1e6 / 31;
	constexpr static int64_t MAX_PULSE_us  = 5000;
	constexpr static int     BAUDRATE      = 20000;

	constexpr static int64_t SERIAL_DELAY_AFTER_PULSE = 1000; // 1ms
	constexpr static int64_t SERIAL_SEND_TIME_us      = 2500; // 1ms

	template <typename... Args>
	SlaveCommunicationMultiplexer(Args... args)
	    : d_pins{args...} {
		for (size_t i = 0; i < N; ++i) {
			const auto &p = d_pins[0];
			gpio_set_dir(p, GPIO_OUT);
			gpio_set_function(p, GPIO_FUNC_SIO);
		}
		putPins(false);

		uart_init(uart1, BAUDRATE);
		d_pulseStart  = 0;
		d_serialStart = 0;
	}

	void Work(absolute_time_t now) {

		const auto sincePulseStart_us =
		    absolute_time_diff_us(d_pulseStart, now);

		switch (d_state) {
		case State::IDLE:
			break;
		case State::PULSE:
			if (sincePulseStart_us >= MAX_PULSE_us) {
				Warnf("[Helios/Comm]: Pulse too long, stopping.");
				EndPulse();
			}
			break;
		case State::PULSE_BREAK:
			if (d_nextValue != nullptr &&
			    sincePulseStart_us >=
			        (MAX_PULSE_us + SERIAL_DELAY_AFTER_PULSE)) {
				Debugf("[Helios/Comm]: Sending serial data");
				sendValue(now);
			}
			if (sincePulseStart_us >= MIN_PERIOD_us) {
				rearm();
			}
			break;
		case State::SENDING_SERIAL:
			if (absolute_time_diff_us(d_serialStart,now) >= (SERIAL_SEND_TIME_us)) {
				if (sincePulseStart_us >= MIN_PERIOD_us) {
					rearm();
				} else {
					d_state = State::PULSE_BREAK;
				}
			}
			break;
		}
	}

	void StartPulse() {
		if (d_state != State::IDLE) {
			const auto now = get_absolute_time();
			const auto ellapsed =
			    Duration(absolute_time_diff_us(d_pulseStart, now));
			Warnf("[Helios/Comm]: Skipping pulse: too high frequency");
			Infof(
			    "[Helios/Comm]: now:%llu.%llu pulse start: %llu.%llu %s ago",
			    now / 1000000,
			    now % 1000000,
			    d_pulseStart / 1000000,
			    d_pulseStart % 1000000,
			    FormatDuration(ellapsed).c_str()
			);
			return;
		}
		if (d_inhibited == true) {
			Warnf("[Helios/Comm]: Skipping pulse: inhibited");
			return;
		}
		Debugf("[Helios]: Starting pulse");

		d_state = State::PULSE;
		putPins(true);

		auto now     = get_absolute_time();
		d_pulseStart = now;
	}

	void EndPulse() {
		if (d_state != State::PULSE) {
			return;
		}
		putPins(false);

		d_state = State::PULSE_BREAK;
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
		d_nextValue = std::make_unique<ArkeHeliosSetPoint_t>(value);
		if (d_state != State::IDLE) {
			return;
		}
		d_state = State::PULSE_BREAK;
		sendValue(get_absolute_time());
	}

private:
	enum class State {
		IDLE,        // Serial allowed.
		PULSE,       // Sending pulse // Serial allowed
		PULSE_BREAK, // Break, Serial maybe soon.
		SENDING_SERIAL,
	};

	void sendValue(absolute_time_t now) {
		if (d_nextValue == nullptr) {
			return;
		}

		d_serialStart = now;

		Debugf("[Helios/comm] sending data from %llu.%llu",now/1000000,now%1000000);
		d_state       = State::SENDING_SERIAL;
		gpio_set_function(d_pins[0], UART_FUNCSEL_NUM(1, d_pins[0]));
		d_buffer[0] = 0x55;
		d_buffer[1] = d_nextValue->Visible;
		d_buffer[2] = d_nextValue->UV;
		uart_write_blocking(uart1, (const uint8_t *)(d_buffer), 3);
		d_nextValue = nullptr;
	}

	void rearm() {
		gpio_set_function(d_pins[0], GPIO_FUNC_SIO);
		d_state = State::IDLE;
		Debugf("[Helios/comm]: ready to fire.");
	}

	void putPins(bool high) {
		for (size_t i = 0; i < N; ++i) {
			// pin 0 is inverted
			gpio_put(d_pins[i], i == 0 ? !high : high);
		}
	}

	bool d_inhibited = false;

	State d_state = State::IDLE;

	uint d_txPin, outPin;

	std::unique_ptr<ArkeHeliosSetPoint_t> d_nextValue;
	char                                d_buffer[3];
	std::array<uint, N>                 d_pins;
	absolute_time_t                     d_pulseStart, d_serialStart;
};

static std::unique_ptr<LED> green, yellow;

class Helios {
public:
	struct Args {
		uint SyncOutPin, InIsolatedPin, InTTLPin, TxPin, StrobePin, InhibitPin;
	};

	constexpr static int64_t VISIBLE_UPDATE_PERIOD_US = 200 * 1000;
	constexpr static int64_t SYNC_SIGNAL_US           = 1000;

	Helios(Args &&args)
	    : d_slaves{args.TxPin, args.StrobePin}
	    , d_pins{std::move(args)}
	    , d_lastVisibleUpdate(get_absolute_time() - VISIBLE_UPDATE_PERIOD_US) {

		lock_init(&d_lock,next_striped_spin_lock_num());

		gpio_init(d_pins.SyncOutPin);
		gpio_set_dir(d_pins.SyncOutPin, GPIO_OUT);
		gpio_put(d_pins.SyncOutPin, 0);

		gpio_init(d_pins.InIsolatedPin);
		gpio_init(d_pins.InTTLPin);
		gpio_init(d_pins.InTTLPin);

	}

	ArkeHeliosSetPoint_t Visible() {
		return {.Visible = d_echoConfig.Visible, .UV = d_echoConfig.UV};
	}

	void SetVisible(const ArkeHeliosSetPoint_t &value) {

		d_echoConfig.Visible    = value.Visible;
		d_echoConfig.UV         = value.UV;


		auto saved = spin_lock_blocking(d_lock.spin_lock);
		d_newSetPoint = std::make_unique<ArkeHeliosSetPoint_t>(value);
		spin_unlock(d_lock.spin_lock, saved);
	}

	uint64_t VisiblePulsation_us() {
		return d_echoConfig.VisiblePulsationPeriod_us;
	}

	ArkeHeliosTriggerConfig_t Trigger() {
		return ArkeHeliosTriggerConfig_t{
		    .Period_hecto_us = uint16_t(d_echoConfig.PulsePeriod_us / 100),
		    .Pulse_us        = uint16_t(d_echoConfig.PulseLength_us),
		};
	}

	void SetTrigger(const ArkeHeliosTriggerConfig_t &config) {
		d_echoConfig.PulsePeriod_us = config.Period_hecto_us * 100;
		d_echoConfig.PulseLength_us = config.Pulse_us;

		auto saved = spin_lock_blocking(d_lock.spin_lock);
		d_newTrigger = std::make_unique<ArkeHeliosTriggerConfig_t>(config);
		spin_unlock(d_lock.spin_lock, saved);
	}

	void Work() {
		// d_slaves.SetInhibited(gpio_get(d_pins.InhibitPin));

		auto now = get_absolute_time();
		d_slaves.Work(now);

		bool hasTrigger =
		    gpio_get(d_pins.InIsolatedPin) || gpio_get(d_pins.InTTLPin);

		if (d_workConfig.PulsePeriod_us > 0) {
			auto canPull = generatePulse(now);
			if ( canPull == true ) {
				pullUpdates();
			}
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
			pullUpdates();
		}

		if (absolute_time_diff_us(d_lastVisibleUpdate, now) >
		    VISIBLE_UPDATE_PERIOD_US) {
			Debugf("[Helios]: Visible set point update");
			d_slaves.SetPoint({.Visible = d_workConfig.Visible, .UV = d_workConfig.UV});
			d_lastVisibleUpdate = now;
		}
	}



private:
	struct Config {
		uint8_t Visible                   = 0;
		uint8_t UV                        = 0;
		int64_t VisiblePulsationPeriod_us = 0;
		int64_t PulsePeriod_us            = 0;
		int64_t PulseLength_us            = 0;
	};




	void pullUpdates() {
		std::unique_ptr<ArkeHeliosSetPoint_t> newSetPoint;
		std::unique_ptr<ArkeHeliosTriggerConfig_t> newTriggerConfig;


		auto saved = spin_lock_blocking(d_lock.spin_lock);
		newSetPoint = std::move(d_newSetPoint);
		newTriggerConfig = std::move(d_newTrigger);
		spin_unlock(d_lock.spin_lock, saved);

		if (newSetPoint != nullptr) {
			Infof("[Helios]: Setting visible point to Visible:%d UV:%d",
				  newSetPoint->Visible, newSetPoint->UV);

			d_slaves.SetPoint(*newSetPoint);
			d_lastVisibleUpdate = get_absolute_time();
			d_workConfig.Visible = newSetPoint->Visible;
			d_workConfig.UV = newSetPoint->UV;
		}

		if ( newTriggerConfig == nullptr ){
			return;
		}

		bool triggerNow = d_workConfig.PulsePeriod_us <= 0;
		d_workConfig.PulsePeriod_us = newTriggerConfig->Period_hecto_us * 100;
		d_workConfig.PulseLength_us = newTriggerConfig->Pulse_us;


		if (d_workConfig.PulsePeriod_us > 0) {
			Infof(
			    "[Helios]: starting generating pulses, length: %s, period: %s",
			    FormatDuration(Duration(d_workConfig.PulseLength_us)).c_str(),
			    FormatDuration(Duration(d_workConfig.PulsePeriod_us)).c_str()
			);
		} else {
			Infof("[Helios]: using external pulse trigger");
		}

		if ( triggerNow ) {
			d_lastPulse = get_absolute_time() - d_workConfig.PulsePeriod_us;
		}

	}

	bool generatePulse(absolute_time_t now) {
		auto ellapsed = absolute_time_diff_us(d_lastPulse, now);
		auto canPull = false;
		if (d_onPulse && ellapsed > d_workConfig.PulseLength_us) {
			d_slaves.EndPulse();
			Debugf("[Helios/generate]: Pulse End");
			d_onPulse = false;
			canPull = true;
		}
		if (d_onSync && ellapsed > SYNC_SIGNAL_US) {
			gpio_put(d_pins.SyncOutPin, 0);
			Debugf("[Helios/generate]: Sync End");
			d_onSync = false;
		}


		if (d_onPulse == false && d_onSync == false &&
		    ellapsed > d_workConfig.PulsePeriod_us) {
			d_slaves.StartPulse();
			d_onPulse = true;
			d_onSync  = true;
			gpio_put(d_pins.SyncOutPin, 1);
			d_lastPulse += d_workConfig.PulsePeriod_us;
			Debugf("[Helios/generate]: Pulse and Sync Start");
		}
		return canPull;
	}

	SlaveCommunicationMultiplexer<2> d_slaves;
	Args                             d_pins;


	lock_core_t d_lock;

	Config d_echoConfig,d_workConfig;

	std::unique_ptr<ArkeHeliosTriggerConfig_t> d_newTrigger;
	std::unique_ptr<ArkeHeliosSetPoint_t> d_newSetPoint;

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
constexpr static uint TCAN_RX       = 29;
constexpr static uint TCAN_TX       = 28;
constexpr static uint DRIVER_EN     = 2;

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


static void core1_task() {
	multicore_lockout_victim_init();

	for (;;) {
		helios.Work();
	}
}



void init() {
	stdio_init_all();

	//Logger::InitLogsOnSecondCore();


	multicore_launch_core1(&core1_task);

	while(multicore_lockout_victim_is_initialized(1) == false ){
		tight_loop_contents();
	}

#ifndef NDEBUG
	Logger::Get().SetLevel(Logger::Level::DEBUG);
#endif

	green  = std::make_unique<LED>(11);
	yellow = std::make_unique<LED>(12);




	gpio_init(TCAN_SHUTDOWN);
	gpio_init(TCAN_STANDBY);
	gpio_init(DRIVER_EN);
	gpio_set_dir(TCAN_SHUTDOWN, GPIO_OUT);
	gpio_set_dir(TCAN_STANDBY, GPIO_OUT);
	gpio_set_dir(DRIVER_EN, GPIO_OUT);
	gpio_put(TCAN_SHUTDOWN, 0);
	gpio_put(TCAN_STANDBY, 0);
	gpio_put(DRIVER_EN, 1);

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

	Scheduler::Schedule(200, 1000, [](absolute_time_t now) {
		Logger::FormatsMaybePendingLogs();
		return std::nullopt;
	});
}

int main() {
	init();

#ifndef NDEBUG
	Scheduler::Schedule(1, 1000000, [](absolute_time_t) {
		Infof("Alive!");
		return std::nullopt;
	});
	green->Set(255, 2 * 1000 * 1000);
	ArkeScheduleStats(2000000);
#endif

	for (;;) {
		Scheduler::Work();
	}
	return 0;
}

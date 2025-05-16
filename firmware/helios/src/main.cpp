#include <cstdint>
#include <functional>
#include <hardware/pio.h>

extern "C" {
#include <hardware/sync.h>
#include <hardware/gpio.h>
#include <hardware/uart.h>
#include <pico/lock_core.h>
#include <pico/stdlib.h>
#include <pico/time.h>
#include <pico/types.h>
#include <pico/multicore.h>
#include "pulse.pio.h"
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


static std::unique_ptr<LED> green, yellow;

class Helios {
public:
	struct Args {
		uint StrobePin, TxPin;
	};


	Helios(Args &&args)
		: d_pins{args.StrobePin,args.TxPin}{

		d_sms[0] = pio_claim_unused_sm(d_pio, true);
		d_sms[1] = pio_claim_unused_sm(d_pio, true);

		gpio_set_outover (d_pins[1], GPIO_OVERRIDE_INVERT);

		d_offset = pio_add_program(d_pio, &pulse_program);

		for ( uint p : d_pins) {
			gpio_set_dir(p,GPIO_OUT);
			gpio_put(p,false);
		}

		stop();
	}

	ArkeHeliosSetPoint_t Visible() {
		return {.Visible = 0, .UV = 0};
	}

	void SetVisible(const ArkeHeliosSetPoint_t &value) {
		Warnf("[Helios]: Visible management is not yet supported!");
	}

	uint64_t VisiblePulsation_us() {
		return 0;
	}

	ArkeHeliosTriggerConfig_t Trigger() {
		return ArkeHeliosTriggerConfig_t{
		    .Period_hecto_us = uint16_t(d_config.PulsePeriod_us / 100),
		    .Pulse_us        = uint16_t(d_config.PulseLength_us),
		};
	}

	void SetTrigger(const ArkeHeliosTriggerConfig_t &config) {
		if ( config.Period_hecto_us == 0 ) {
			d_config.PulsePeriod_us = 0;
			d_config.PulseLength_us = 0;
			Infof("[Helios]: not triggering");
			stop();
			return;
		}

		bool needStart = d_config.PulsePeriod_us == 0;
		if ( needStart == false && config.Period_hecto_us * 100 != d_config.PulsePeriod_us ) {
			Errorf("[Helios]: Can only change period while not running, skipping update");
			return;
		}

		d_config.PulseLength_us = config.Pulse_us;
		d_config.PulsePeriod_us = 100 * config.Period_hecto_us;

		if ( config.CameraDelay_us != 0 ) {
			Warnf("[Helios]: Camera delay is not supported");
		}


		Infof("[Helios]: triggering %lld.%03lldms pulse every %lld.%03lldms",
			  d_config.PulseLength_us/1000,d_config.PulseLength_us%1000,
			  d_config.PulsePeriod_us/1000,d_config.PulsePeriod_us%1000);
		if ( needStart) {
			start();
		} else {
			updatePeriod();
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


	void stop(){
		for ( uint p : d_pins) {
			gpio_set_function(p, GPIO_FUNC_SIO);
		}
		gpio_set_outover(d_pins[1], GPIO_OVERRIDE_INVERT);
	};
	void start(){
		uint mask;
		for ( uint i = 0; i < 2; i++) {
			pulse_program_init(d_pio, d_sms[i], d_offset, d_pins[i]);
			pulse_program_set_period(d_pio, d_sms[i], d_config.PulsePeriod_us);
			mask |= ( 1u << d_sms[i] );
		}
		gpio_set_outover(d_pins[1], GPIO_OVERRIDE_INVERT);
		updatePeriod();
		pio_enable_sm_mask_in_sync(d_pio, mask);
	};

	void updatePeriod(){
		for ( uint sm : d_sms ) {
			pulse_program_set_pulse(d_pio, sm, d_config.PulseLength_us);
		}
	}

	Config d_config;
	// Arke is using pio0
	PIO d_pio = pio1;
	uint d_sms[2],d_offset,d_pins[2],d_smMasks;
};


static Helios helios{Helios::Args{
	    .StrobePin     = 24,
		.TxPin = 8,
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
		Debugf("[ARKE]: Unexpected message 0x%03X DLC:%d", e.Class, e.Size);
	}
}


static void core1_task() {
	multicore_lockout_victim_init();

	for (;;) {
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

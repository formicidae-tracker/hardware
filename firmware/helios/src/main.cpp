#include <cstdint>
#include <functional>
#include <hardware/irq.h>
#include <hardware/pio.h>
#include <hardware/regs/intctrl.h>
#include <hardware/structs/io_bank0.h>

extern "C" {
#include "pulse.pio.h"
#include <hardware/gpio.h>
#include <hardware/sync.h>
#include <hardware/uart.h>
#include <pico/lock_core.h>
#include <pico/multicore.h>
#include <pico/stdlib.h>
#include <pico/time.h>
#include <pico/types.h>
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
// // 2. If UART for visible is needed, we should multiplex in software the
// output
// // using a PIO to detect an end of pulse and check when it is safe to
// multiplex
// // (no need now) by switching the pin to UART.
// //

// //
//
// 4. check for inhibit and trigger input support via PIO or interrupt (maybe
// interrupt from another core is better??)
//

static std::unique_ptr<LED> green, yellow;

constexpr pio_interrupt_source relative_PIO_interrupt(uint sm, uint interrupt) {
	uint index = (interrupt & 0x03 + sm & 0x03) % 4;
	return static_cast<pio_interrupt_source>(pis_interrupt0 + index);
}

void helios_irq_handler(void);

inline bool uart_tx_busy(uart_inst_t *uart) {
	return (uart_get_hw(uart)->fr & UART_UARTFR_BUSY_BITS) != 0x00;
}

class Helios {
public:
	struct Args {
		uint CameraPin, TxPin, SyncPin;
	};

	constexpr static size_t  FRAME_SIZE          = 3;
	constexpr static int     BAUDRATE            = 20000;
	constexpr static int64_t MIN_PERIOD_us       = 31250;
	constexpr static int64_t MAX_PULSE_us        = 4500;
	constexpr static int64_t UPDATE_PERIOD_US    = 40000;
	constexpr static int64_t SERIAL_DELAY_us     = 1000;
	// adds a 1ms margin to send time.
	constexpr static int64_t SERIAL_SEND_TIME_us =
	    1000000 * FRAME_SIZE / BAUDRATE + 1000;

	Helios(Args &&args)
	    : d_pio{pio1}
	    , d_camera{.Pin = args.CameraPin, .SM = 1}
	    , d_tx{.Pin = args.TxPin, .SM = 0}
	    , d_sync{.Pin = args.SyncPin, .SM = 2} {

		uart_init(uart1, BAUDRATE);
		pio_sm_claim(d_pio, d_camera.SM);
		pio_sm_claim(d_pio, d_tx.SM);
		pio_sm_claim(d_pio, d_sync.SM);

		gpio_set_outover(d_camera.Pin, GPIO_OVERRIDE_INVERT);
		gpio_set_outover(d_sync.Pin, GPIO_OVERRIDE_INVERT);

		d_offset = pio_add_program(d_pio, &pulse_program);

		for (uint p : {d_camera.Pin, d_sync.Pin, d_tx.Pin}) {
			gpio_set_dir(p, GPIO_OUT);
			gpio_put(p, true);
			gpio_set_function(p, GPIO_FUNC_SIO);
		}

		stopPulsing();

		pio_set_irq0_source_enabled(d_pio, pis_interrupt0, true);
		irq_set_exclusive_handler(PIO1_IRQ_0, helios_irq_handler);
		irq_set_enabled(PIO1_IRQ_0, true);

		gpio_set_function(d_tx.Pin, GPIO_FUNC_UART);
	}

	ArkeHeliosSetPoint_t Visible() {
		return {.Visible = d_config.Visible, .UV = d_config.UV};
	}

	void SetVisible(const ArkeHeliosSetPoint_t &value) {
		Infof("[Helios]: Set Visible:%d, UV:%d", value.Visible, value.UV);
		d_config.Visible = value.Visible;
		d_config.UV      = value.UV;
		if (d_config.PulsePeriod_us == 0) {
			bufferVisibleUpdate(value);
		} // otherwise we wait for the next update.
	}

	uint64_t VisiblePulsation_us() {
		return d_config.VisiblePulsationPeriod_us;
	}

	uint64_t SetVisiblePulsation_us(uint pulsationPeriod_us) {
		return d_config.VisiblePulsationPeriod_us = pulsationPeriod_us;
	}

	ArkeHeliosTriggerConfig_t Trigger() {

		return ArkeHeliosTriggerConfig_t{
		    .Period_hecto_us = uint16_t(d_config.PulsePeriod_us / 100),
		    .Pulse_us        = uint16_t(d_config.PulseLength_us),
		};
	}

	void SetTrigger(const ArkeHeliosTriggerConfig_t &config) {
		if (config.Period_hecto_us == 0) {
			d_config.PulsePeriod_us = 0;
			d_config.PulseLength_us = 0;
			Infof("[Helios]: not triggering");
			stopPulsing();
			return;
		}

		bool needStart = d_config.PulsePeriod_us == 0;
		if (needStart == false &&
		    config.Period_hecto_us * 100 != d_config.PulsePeriod_us) {
			Errorf(
			    "[Helios]: Can only change period while not running, skipping "
			    "update"
			);
			return;
		}

		d_config.PulseLength_us = config.Pulse_us;
		d_config.PulsePeriod_us = 100 * config.Period_hecto_us;

		if (config.CameraDelay_us != 0) {
			Warnf("[Helios]: Camera delay is not supported");
		}

		Infof(
		    "[Helios]: triggering %lld.%03lldms pulse every %lld.%03lldms",
		    d_config.PulseLength_us / 1000,
		    d_config.PulseLength_us % 1000,
		    d_config.PulsePeriod_us / 1000,
		    d_config.PulsePeriod_us % 1000
		);
		if (needStart) {
			startPulsing();
		} else {
			updatePulse();
		}
	}

	void Work() {
		auto now = get_absolute_time();

		if (d_config.PulsePeriod_us != 0) {
			workPulsing(now);
		} else {
			workIdle();
		}

		workUpdateVisible(now);
	}

private:
	struct Config {
		uint8_t Visible                   = 0;
		uint8_t UV                        = 0;
		int64_t VisiblePulsationPeriod_us = 0;
		int64_t PulsePeriod_us            = 0;
		int64_t PulseLength_us            = 0;
	};

	void workUpdateVisible(absolute_time_t now) {
		// we periodically send the Data.
		auto ellapsed = absolute_time_diff_us(d_lastUpdate, now);
		if (ellapsed < UPDATE_PERIOD_US) {
			return;
		}

		if (d_config.VisiblePulsationPeriod_us > 0) {
			float phase = float(now % (d_config.VisiblePulsationPeriod_us)) /
			              float(d_config.VisiblePulsationPeriod_us);
			if (phase > 0.5) {
				phase = 1.0 - phase;
			}
			phase *= 2.0;
			bufferVisibleUpdate(
			    {.Visible = uint8_t(d_config.Visible * phase),
			     .UV      = uint8_t(d_config.UV * phase)}
			);
		} else {
			bufferVisibleUpdate({.Visible = d_config.Visible, .UV = d_config.UV}
			);
		}
		d_lastUpdate += UPDATE_PERIOD_US;
	}

	void workPulsing(absolute_time_t now) {

		// first we disable UART if we are running it.
		if (gpio_get_function(d_tx.Pin) == GPIO_FUNC_UART) {
			if (uart_tx_busy(uart1) == false) {
				gpio_set_function(d_tx.Pin, PIO_FUNCSEL_NUM(d_pio, d_tx.Pin));
			}
			return;
		}

		// if we do not have a value, just exit.
		if (d_sendBuffer.has_value() == false) {
			return;
		}

		auto status     = save_and_disable_interrupts();
		auto pulseStart = d_lastPulse;
		restore_interrupts_from_disabled(status);

		// if free, and we have a time window for an upload, let's go !!
		auto sincePulseEnd = absolute_time_diff_us(pulseStart, now);

		//
		// This threshold is for long pulse periods. We should not trigger any
		// change before the rearm time. However for short pulses it can becomes
		// smaller than SERIAL_DELAY_us int64_t
		// longPulseThreshold_us = d_config.PulsePeriod_us -
		// d_config.PulseLength_us - SERIAL_SEND_TIME_us - 2 * MIN_PERIOD_us;

		// the other threshold is for small pulses. It is always safe to send
		// while in the rearm windows of the last pulse.
		constexpr static int64_t SERIAL_AFTER_PULSE_THRESHOLD_us =
		    MIN_PERIOD_us - MAX_PULSE_us - SERIAL_SEND_TIME_us;

		// NOTE: using a too clever strategy create ghost pulses in Nyx
		// board. it is simpler to just authorise a TX after only just after a
		// pulse.
		auto upperThreshold_us = SERIAL_DELAY_us + 4000;

		static int i = 0;

		if (sincePulseEnd >= SERIAL_DELAY_us &&
		    sincePulseEnd < upperThreshold_us && //
		    uart_tx_busy(uart1) == false) {
			startTx();
		}
	}

	void workIdle() {
		// if we have something to send and are not sending, we start.
		if (d_sendBuffer.has_value() == false || uart_tx_busy(uart1) == true) {
			return;
		}
		startTx();
	}

	void stopPulsing() {
		gpio_set_function(d_camera.Pin, GPIO_FUNC_SIO);
		gpio_set_outover(
		    d_camera.Pin,
		    GPIO_OVERRIDE_INVERT
		); // may not be needed.
		gpio_set_function(d_tx.Pin, GPIO_FUNC_UART);
		gpio_set_function(d_sync.Pin, GPIO_FUNC_SIO);
		gpio_set_outover(
		    d_sync.Pin,
		    GPIO_OVERRIDE_INVERT
		); // may not be needed.
	};

	void startPulsing() {
		uint mask;
		uart_tx_wait_blocking(uart1
		); // ensure the UART is done before reconfiguring any pin.

		for (const auto &d : {d_camera, d_tx, d_sync}) {
			pulse_program_init(d_pio, d.SM, d_offset, d.Pin);
			pulse_program_set_period(d_pio, d.SM, d_config.PulsePeriod_us);
			mask |= (1u << d.SM);
		}

		// maynot be needed, but repeat that we want inverted pins.
		gpio_set_outover(d_camera.Pin, GPIO_OVERRIDE_INVERT);
		gpio_set_outover(d_sync.Pin, GPIO_OVERRIDE_INVERT);

		updatePulse();

		// we ensure that we won't trigger before actually having
		// triggerred. Will be updated by the interrupt to the actual timestamp
		// once fired.
		d_lastPulse = make_timeout_time_us(4 * d_config.PulsePeriod_us);

		pio_enable_sm_mask_in_sync(d_pio, mask);
	};

	void bufferVisibleUpdate(ArkeHeliosSetPoint_t sp) {
		d_sendBuffer = sp;
	}

	void startTx() {
		if (d_sendBuffer.has_value() == false) {
			return;
		}

		auto sp = d_sendBuffer.value();
		gpio_set_function(d_tx.Pin, GPIO_FUNC_UART);
		uint8_t buffer[3];
		buffer[0] = 0x55;
		buffer[1] = sp.Visible;
		buffer[2] = sp.UV;
		uart_write_blocking(uart1, buffer, 3);

		d_sendBuffer = std::nullopt;
	}

	void updatePulse() {
		pulse_program_set_pulse(d_pio, d_camera.SM, d_config.PulseLength_us);
		pulse_program_set_pulse(d_pio, d_tx.SM, d_config.PulseLength_us);
		pulse_program_set_pulse(d_pio, d_sync.SM, 1000000);
	}

	Config d_config;
	// Arke is using pio0
	PIO    d_pio;

	struct SMConfig {
		uint Pin, SM;
	};

	friend void helios_irq_handler(void);

	SMConfig        d_camera, d_tx, d_sync;
	uint            d_offset;
	absolute_time_t d_lastPulse = -1, d_lastUpdate = 0;

	std::optional<ArkeHeliosSetPoint_t> d_sendBuffer;
	bool                                d_sending = false;
};

static Helios helios{Helios::Args{
    .CameraPin = 24,
    .TxPin     = 8,
    .SyncPin   = 25,
}};

void __not_in_flash_func(helios_irq_handler)(void) {
	pio_interrupt_clear(pio1, 0);
	helios.d_lastPulse = get_absolute_time();
}

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
		Warnf("[HELIOS]: Not Yet Implemented");
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

	// Logger::InitLogsOnSecondCore();

	multicore_launch_core1(&core1_task);

	while (multicore_lockout_victim_is_initialized(1) == false) {
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
		helios.Work();
		Scheduler::Work();
	}
	return 0;
}

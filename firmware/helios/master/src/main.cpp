#include <hardware/gpio.h>

#include <hardware/uart.h>
#include <optional>
#include <pico/stdlib.h>
#include <pico/types.h>

#include <utils/Log.hpp>

#include "LED.hpp"
#include "arke.h"
#include "utils/Scheduler.hpp"

template <size_t N> class SlaveCommunicationMultiplexer {
public:
	constexpr static int64_t MIN_PERIOD = 1e6 / 30.5;
	constexpr static int64_t MAX_PULSE  = 4000;
	constexpr static int     BAUDRATE   = 20000;

	template <typename... Args>
	SlaveCommunicationMultiplexer(Args... args)
	    : d_pins{args...} {
		for (size_t i = 0; i < N; ++i) {
			const auto &p = d_pins[0];
			gpio_set_dir(p, GPIO_OUT);
			gpio_put(p, i == 0 ? true : false);
			gpio_set_func(p, GPIO_FUNC_SIO);
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

		Scheduler::Schedule(0, -MAX_PULSE, [this](absolute_time_t) {
			EndPulse();
			return -1;
		});
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
		Scheduler::Schedule(0, MAX_PULSE - MIN_PERIOD, [this](absolute_time_t) {
			rearm();
			return -1;
		});
	}

	void InhibitPulse() {
		d_inhibited = true;
		EndPulse();
	}

	void DeinhibitPulse() {
		d_inhibited = false;
	}

	void SetPoint(ArkeHeliosSetPoint_t value) {
		d_nextValue = value;
		if (d_state != State::IDLE) {
			return;
		}
		d_state = State::PULSE_BREAK;
		sendValue();
		Scheduler::Schedule(0, -1000, [this] {
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
		d_buffer[4] = 0x00;
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

int main() {
	stdio_init_all();

	Logger::InitLogsOnSecondCore();
#ifndef NDEBUG
	Logger::Get().SetLevel(Logger::Level::DEBUG);
#endif
	LED green{11}, yellow{12};

	green.Set(100, 2 * 1000 * 1000);
	yellow.Blink(3);

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

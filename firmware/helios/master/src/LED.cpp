#include "LED.hpp"

#include <cstdint>
#include <hardware/gpio.h>
#include <hardware/pwm.h>

#include <pico/types.h>

#include <utils/Log.hpp>
#include <utils/Scheduler.hpp>

LED::LED(uint pin)
    : d_slice{pwm_gpio_to_slice_num(pin)}
    , d_channel{pwm_gpio_to_channel(pin)} {

	auto config = pwm_get_default_config();
	pwm_config_set_clkdiv(&config, float(SYS_CLK_HZ) / 255 * 1000.0f);
	pwm_config_set_wrap(&config, 255 - 1);
	gpio_set_function(pin, GPIO_FUNC_PWM);
	pwm_set_chan_level(d_slice, d_channel, 0);
	pwm_init(d_slice, &config, true);

#ifdef NDEBUG
	constexpr static uint64_t PERIOD = 25000;
#else
	constexpr static uint64_t PERIOD = 250000;
#endif

	Scheduler::Schedule(10, PERIOD, [this](absolute_time_t now) {
		return work(now);
	});
}

void LED::Set(uint8_t level, uint pulsePeriod_us) {
	d_level          = level;
	d_blinkCount     = 0;
	d_pulsePeriod_us = pulsePeriod_us;
	if (pulsePeriod_us == 0) {
		pwm_set_chan_level(d_slice, d_channel, level);
	}
}

void LED::Blink(uint count, uint8_t level) {
	if (count == 0) {
		Set(0);
		return;
	}
	d_level          = level;
	d_pulsePeriod_us = 0;
	d_blinkCount     = count;
}

void LED::performPulse(absolute_time_t now) {
	if (d_pulsePeriod_us == 0) {
		return;
	}
	uint8_t target = d_level;
	uint    phase  = (now % d_pulsePeriod_us) * 2;
	if (phase >= d_pulsePeriod_us) {
		phase = std::max(0U, 2 * d_pulsePeriod_us - phase);
	}

	target = phase * uint(d_level) / d_pulsePeriod_us;

	Debugf(
	    "[LED %d:%d]: phase: %d period: %d (%.2f%%) target: %d",
	    d_slice,
	    d_channel,
	    phase,
	    d_pulsePeriod_us,
	    100.0f * float(phase) / d_pulsePeriod_us,
	    target
	);
	pwm_set_chan_level(d_slice, d_channel, target);
}

constexpr static int BLINK_PULSE_LENGTH_US = 250 * 1000;

void LED::performBlink(absolute_time_t now) {
	uint phase = (now / BLINK_PULSE_LENGTH_US) % (2 * (d_blinkCount + 1));
	if (phase >= 2 * d_blinkCount) {
		pwm_set_chan_level(d_slice, d_channel, 0);
		return;
	}
	pwm_set_chan_level(d_slice, d_channel, (phase % 2) == 0 ? d_level : 0);
}

std::optional<int64_t> LED::work(absolute_time_t now) {
	if (d_blinkCount > 0) {
		performBlink(now);
	} else {
		performPulse(now);
	}

	return std::nullopt;
}

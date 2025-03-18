#pragma once

#include <stdint.h>

typedef uint16_t absolute_time_t;

void init_system_clock();

void system_clock_stop();
void system_clock_start();

absolute_time_t get_absolute_time();

constexpr static uint32_t SYS_CLK_HZ = 32768;

inline constexpr absolute_time_t TicksToMS(absolute_time_t v) {
	return uint32_t(v) * 1000ULL / SYS_CLK_HZ;
}

inline constexpr absolute_time_t USToTicks(uint16_t us) {
	return SYS_CLK_HZ * uint32_t(us) / 1000000ULL;
}

inline constexpr absolute_time_t MSToTicks(float ms) {
	return ms * 32.768f;
}

inline constexpr absolute_time_t MSToTicks(absolute_time_t ms) {
	return SYS_CLK_HZ * uint32_t(ms) / 10000ULL;
}

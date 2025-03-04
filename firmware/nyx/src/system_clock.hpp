#pragma once

#include <stdint.h>

typedef uint16_t absolute_time_t;

void init_system_clock();

absolute_time_t get_absolute_time();

constexpr static uint32_t SYS_CLK_HZ  = F_CPU;
constexpr static uint32_t SYS_CLK_kHZ = F_CPU / 1000;
constexpr static uint32_t SYS_CLK_MHZ = F_CPU / 1000000;

inline constexpr absolute_time_t TicksToMS(absolute_time_t v) {
	return (v << 8) / SYS_CLK_kHZ;
}

inline constexpr absolute_time_t MSToTicks(float v) {
	return v * SYS_CLK_kHZ / 256.0;
}

inline constexpr absolute_time_t MSToTicks(absolute_time_t v) {
	return (uint32_t(v) * SYS_CLK_kHZ) >> 8;
}

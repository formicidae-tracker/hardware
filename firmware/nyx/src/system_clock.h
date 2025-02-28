#pragma once


#include <stdint.h>

typedef uint8_t absolute_time_t;

void init_system_clock();

#define TICK_PER_MS 4

absolute_time_t get_absolute_time();

#define time_ms_to_ticks(a) ((a) << 2)
#define time_ticks_to_ms(a) ((a) >> 2)

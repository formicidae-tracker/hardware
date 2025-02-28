#include "system_clock.h"

#include <avr/io.h>


void init_system_clock() {}

absolute_time_t get_absolute_time() {
	return TCA0.SINGLE.CNT;
}

#include "system_clock.hpp"

#include "defer.hpp"

#include <avr/interrupt.h>
#include <avr/io.h>

void init_system_clock() {
	TCA0.SINGLE.PER   = 0xffff;        // Whole clock arround
	TCA0.SINGLE.CTRLA = 0X06 << 1 | 1; // F/256 enabled
}

absolute_time_t get_absolute_time() {
	cli();
	defer {
		sei();
	};
	return TCA0.SINGLE.CNT;
}

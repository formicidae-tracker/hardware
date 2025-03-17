#include "system_clock.hpp"

#include "defer.hpp"

#include <avr/interrupt.h>
#include <avr/io.h>

void init_system_clock() {
	TCA0.SINGLE.PER = 0xffff; // Whole clock arround
	TCA0.SINGLE.CTRLA =
	    TCA_SINGLE_CLKSEL_DIV64_gc | TCA_SINGLE_ENABLE_bm; // F/64 enabled
}

absolute_time_t get_absolute_time() {
	cli();
	defer {
		sei();
	};
	return TCA0.SINGLE.CNT;
}

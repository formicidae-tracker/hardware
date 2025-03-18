#include "system_clock.hpp"

#include "defer.hpp"

#include <avr/interrupt.h>
#include <avr/io.h>

void system_clock_stop() {
	for (;;) {
		if ((RTC.STATUS & RTC_CTRLABUSY_bm) != 0) {
			continue;
		}
		RTC.CTRLA = RTC_PRESCALER_DIV1_gc;
		break;
	}
	for (;;) {
		if ((RTC.STATUS & RTC_CNTBUSY_bm) != 0) {
			continue;
		}
		RTC.CNT = 0x0000;
		break;
	}
}

void system_clock_start() {
	for (;;) {
		if ((RTC.STATUS & RTC_CTRLABUSY_bm) != 0) {
			continue;
		}
		RTC.CTRLA = RTC_PRESCALER_DIV1_gc | RTC_RTCEN_bm;
		break;
	}
}

void init_system_clock() {
	RTC.CLKSEL = RTC_CLKSEL_INT32K_gc;
	RTC.PER    = 0xffff;
	RTC.CNT    = 0x0000;
}

absolute_time_t get_absolute_time() {
	return RTC.CNT;
}

#include "Systime.h"

#include <avr/io.h>
#include <avr/interrupt.h>

volatile Systime_t systime;

void InitSystime() {
	systime = 0;

	// frequency is 16000000 / (N *(1 + OCRnA))
	OCR0A = 249;
	TIMSK0 = _BV(OCIE0A);
	TCCR0A = _BV(WGM01) | _BV(CS01) | _BV(CS00); \
	sei();
}

ISR(TIMER0_COMP_vect) {
	++systime;
}
Systime_t GetSystime() {
	uint8_t sreg = SREG;
	cli();
	Systime_t res = systime;
	SREG = sreg;
	return res;
}

#include "Systime.h"

#include <avr/io.h>
#include <avr/interrupt.h>

volatile Systime_t systime;

void InitSystime() {
	systime = 0;

	// frequency is 16000000 / (N *(1 + OCRnA))
	OCR0A = 249;
	TIMSK0 = _BV(1);
	TCCR0A = _BV(WGM01);
	TCCR0B = _BV(CS01) | _BV(CS00);
	sei();
}

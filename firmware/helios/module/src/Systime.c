#include "Systime.h"

#include <avr/io.h>
#include <avr/interrupt.h>

volatile Systime_t systime;


void InitSystime() {
	systime = 0;

	DDRB = _BV(0);
	cli();
	//sets Timer 4 as 1ms clock, aka CTC mode, prescaler at 8 from 20MHz clock, TOP to 2499
	TIMSK4 = _BV(OCIE4A);
	OCR4A = 2499;
	TCCR4A = 0x00;
	TCCR4B = _BV(WGM42) | _BV(CS41); // this will starts the timer

	//starts interrupt
	sei();
}

ISR(TIMER4_COMPA_vect) {
	PORTB ^= _BV(0);
	++systime;
}

Systime_t GetSystime() {
	uint8_t sreg = SREG;
	cli();
	Systime_t res = systime;
	SREG = sreg;
	return res;
}

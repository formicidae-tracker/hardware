#include "LEDs.h"

#include <avr/io.h>

#define implements_led_io(name,port,pin) \
	void LED## name ##On() { \
		PORT ## port |= _BV(pin); \
	} \
	void LED## name ## Off() { \
		PORT ## port &= ~_BV(pin);	  \
	} \
	void LED## name ## Toggle() { \
		PORT ## port ^= _BV(pin); \
	} \

implements_led_io(Ready,B,6)
implements_led_io(Data,B,7)
implements_led_io(Error,B,5)


void InitLEDs() {
	DDRB |= _BV(5) | _BV(6) | _BV(7) ;
}

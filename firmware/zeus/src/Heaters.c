#include "Heaters.h"

#include <avr/io.h>
#include <avr/interrupt.h>



#define set_pwm(comp,pin,value) do {	  \
		if ( value == 0x00 ) { \
			TCCR1A &= ~(_BV(COM ## comp ## 1)); \
			PORTB &= ~(_BV(pin)); \
		} else { \
		uint8_t sreg =SREG; \
		cli(); \
		TCCR1A |= _BV(COM ## comp ## 1); \
		OCR1 ## comp = value; \
		SREG = sreg; \
		} \
	}while(0)

#define set_heat1(power) set_pwm(A,5,power)
#define set_heat2(power) set_pwm(B,6,power)




void InitHeaters() {
	DDRB |= _BV(5) | _BV(6);
	set_heat1(0);
	set_heat2(0);

	TCCR1A = _BV(WGM10);
	TCCR1B = _BV(WGM12) | _BV(CS11);


}


void HeaterSetPower1(uint8_t power) {
	set_heat1(power);
}

void HeaterSetPower2(uint8_t power) {
	set_heat2(power);
}

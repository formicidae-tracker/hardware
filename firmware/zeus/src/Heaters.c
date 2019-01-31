#include "Heaters.h"

#include <avr/io.h>
#include <avr/interrupt.h>





/* #define set_pwm(comp,pin,value) do {	  \ */
/* 		if ( value == 0x00 ) { \ */
/* 			/\*			TCCR1A &= ~(_BV(COM ## comp ## 1));*\/ \ */
/* 			PORTB &= ~(_BV(pin)); \ */
/* 		} else { \ */
/* 			/\*			TCCR1A |= _BV(COM ## comp ## 1); *\/ \ */
/* 			/\*			OCR1 ## comp = value;  *\/ \ */
/* 			PORTB |= (_BV(pin)); \ */
/* 		} \ */
/* 	}while(0) */

/* #define set_heat1(power) set_pwm(A,5,power) */
/* #define set_heat2(power) set_pwm(B,6,power) */

//ugly implementatoin without PWM because it cause racing conditions
//otherwise, anyway we do not need precision and fastness

#define heat1_on() PORTB |= _BV(5)
#define heat1_off() PORTB &= ~(_BV(5))
#define heat2_on() PORTB |= _BV(6)
#define heat2_off() PORTB &= ~(_BV(6))

typedef struct {
	uint8_t Power[2];
	ArkeSystime_t Last;
} Heater_t;

Heater_t H;

void InitHeaters() {
	H.Power[0] = 0;
	H.Power[1] = 0;
	H.Last = ArkeGetSystime();
	DDRB |= _BV(5) | _BV(6);
	heat1_off();
	heat2_off();
}


void HeaterSetPower1(uint8_t power) {
	H.Power[0] = power;
}

void HeaterSetPower2(uint8_t power) {
	H.Power[1] = power;
}


void ProcessHeater(ArkeSystime_t now) {
	uint16_t ellapsed = now-H.Last;
	if ( ellapsed >= 255 ) {
		H.Last = now;
		if (H.Power[0] != 0 ) {
			heat1_on();
		}
		if (H.Power[1] != 0 ) {
			heat2_on();
		}
		return;
	}
	if ( ellapsed >= H.Power[0]) {
		heat1_off();
	}
	if (ellapsed >= H.Power[1] ) {
		heat2_off();
	}
}

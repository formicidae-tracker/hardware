#include "LEDs.h"

#include <arke-avr/systime.h>

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>

#define PULSE_PERIOD_MS 10
#define BLINK_PERIOD_MULTIPLIER 15

#define declare_led_fields(name) \
	uint8_t name ## Target, name ## Count, name ## PulseValue, name ## Increment

typedef struct {
	declare_led_fields(Ready);
	declare_led_fields(Error);
	declare_led_fields(Data);
	ArkeSystime_t last;
	uint8_t multiplier;
} LEDData_t;

LEDData_t LED;

#define MIN(a,b) ( ((a) < (b) ) ? (a) : (b) )
#define MAX(a,b) ( ((a) > (b) ) ? (a) : (b) )
#define CLAMP(value,min,max) MIN(MAX(min,value),max)

#define io_on(port,pin) PORT ## port |= _BV(pin)
#define io_off(port,pin) PORT ## port &= ~(_BV(pin))
#define io_toggle(port,pin) PORT ## port ^= _BV(pin)
#define io_state(port,pin) (PORT ## port & _BV(pin))

#define implements_led_io(name,port,pin,comp)	  \
	void LED ## name ## SetBrightness(uint8_t b) { \
		if (b == 0x00) { \
			TCCR3A &= ~(_BV(COM ## comp ## 1)); \
			io_off(port,pin); \
			return; \
		} \
		uint8_t sreg = SREG; \
		cli(); \
		OCR3 ## comp = b; \
		TCCR3A |= _BV(COM ## comp ## 1); \
		SREG = sreg; \
	} \
	void LED## name ##On() { \
		TCCR3A &= ~(_BV(COM ## comp ## 1)); \
		LED.name ## Target = 0; \
		io_on(port,pin); \
	} \
	void LED## name ## Off() { \
		TCCR3A &= ~(_BV(COM ## comp ## 1)); \
		LED.name ## Target = 0; \
		io_off(port,pin); \
	} \
	void LED## name ## Toggle() { \
		if(LED.name ## Target == 0) { \
			TCCR3A &= ~(_BV(COM ## comp ## 1)); \
			io_toggle(port,pin); \
		} else { \
			LED ## name ## Off(); \
		} \
	} \
	void LED ## name ## Blink( uint8_t count) { \
		TCCR3A &= ~(_BV(COM ## comp ## 1)); \
		LED.name ## Target = CLAMP(MAX(count,LED.name ## Target),1,8); \
		LED.name ## Count = LED.name ## Target * 2; \
	} \
	void LED ## name ## Pulse() { \
		LED.name ## PulseValue = 0; \
		LED.name ## Target = 0xff; \
		LED.name ## Increment = 1; \
	} \
	void LED ## name ## Loop(bool blink) { \
		if (LED.name ## Target == 0) { \
			return; \
		} \
		if (LED.name ## Target == 0xff ) { \
			if ( LED.name ## PulseValue == 0xff) { \
				LED.name ## Increment = 0xff; \
			} else if ( LED.name ## PulseValue == 0x00 ) { \
				LED.name ## Increment = 1; \
			} \
			LED.name ## PulseValue = LED.name ## PulseValue + LED.name ## Increment; \
			LED ## name ## SetBrightness(LED.name ## PulseValue); \
			return; \
		} else if ( !blink ) { \
			return; \
		}\
		if ( LED.name ## Count == LED.name ## Target * 2 ) { \
			LED.name ## Count = 0; \
			io_on(port,pin); \
			return; \
		} \
		if (LED.name ## Count < LED.name ## Target ) { \
			io_toggle(port,pin); \
			if (io_state(port,pin) == 00 ) { \
				++LED.name ## Count; \
			} \
			return; \
		} \
		++LED.name ## Count; \
	} \


implements_led_io(Ready,E,3,A)
implements_led_io(Error,E,4,B)

void InitLEDs() {
	DDRE |= _BV(3) | _BV(4) ;
	TCCR3A = _BV(WGM30);
	TCCR3B = _BV(WGM32) | _BV(CS30);
	LEDReadyOff();
	LEDErrorOff();
	LED.last = 0;
}




void ProcessLEDs() {
	ArkeSystime_t now = ArkeGetSystime();
	if ( (now-LED.last) <  PULSE_PERIOD_MS) {
		return;
	}
	LED.last = now;
	bool blink = false;
	if ( ++LED.multiplier == BLINK_PERIOD_MULTIPLIER ) {
		LED.multiplier = 0;
		blink = true;
	}


	LEDReadyLoop(blink);
	LEDErrorLoop(blink);
}

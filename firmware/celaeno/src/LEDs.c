#include "LEDs.h"

#include "Systime.h"

#include <avr/io.h>

#define BLINK_PERIOD_MS 300
#define BLINK_HALF_PERIOD_MS (BLINK_PERIOD_MS / 2)

#define declare_led_fields(name) \
	uint8_t name ## Target, name ## Count

typedef struct {
	declare_led_fields(Ready);
	declare_led_fields(Error);
	declare_led_fields(Data);
	Systime_t last;
} LEDData_t;

LEDData_t LED;

#define MIN(a,b) ( ((a) < (b) ) ? (a) : (b) )
#define MAX(a,b) ( ((a) > (b) ) ? (a) : (b) )
#define CLAMP(value,min,max) MIN(MAX(min,value),max)

#define io_on(port,pin) PORT ## port |= _BV(pin)
#define io_off(port,pin) PORT ## port &= ~(_BV(pin))
#define io_toggle(port,pin) PORT ## port ^= _BV(pin)
#define io_state(port,pin) (PORT ## port & _BV(pin))

#define implements_led_io(name,port,pin) \
	void LED## name ##On() { \
		LED.name ## Target = 0; \
		io_on(port,pin); \
	} \
	void LED## name ## Off() { \
		LED.name ## Target = 0; \
		io_off(port,pin); \
	} \
	void LED## name ## Toggle() { \
		if(LED.name ## Target == 0) { \
			io_toggle(port,pin); \
		} else { \
			LED ## name ## Off(); \
		} \
	} \
	void LED ## name ## Blink( uint8_t count) { \
		LED.name ## Target = CLAMP(MAX(count,LED.name ## Target),1,8); \
		LED.name ## Count = LED.name ## Target * 2; \
	} \
	void LED ## name ## BlinkLoop() { \
		if (LED.name ## Target == 0) { \
			return; \
		} \
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


implements_led_io(Ready,E,3)
implements_led_io(Error,E,4)

void InitLEDs() {
	DDRE |= _BV(3) | _BV(4) ;
	LEDReadyOff();
	LEDErrorOff();
	LED.last = 0;
}




void ProcessLEDs() {
	Systime_t now = GetSystime();
	if ( (now-LED.last) < BLINK_HALF_PERIOD_MS ) {
		return;
	}
	LED.last = now;

	LEDReadyBlinkLoop();
	LEDErrorBlinkLoop();
}

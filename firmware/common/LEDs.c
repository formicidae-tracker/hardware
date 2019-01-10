#include "LEDs.h"

#ifdef DONT_USE_ARKE
#include "Systime.h"
#define TIME_T Systime_t
#define GET_TIME() GetSystime()
#else
#include <arke-avr/systime.h>
#define TIME_T ArkeSystime_t
#define GET_TIME() ArkeGetSystime()
#endif


#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>

#define PULSE_PERIOD_MS 10
#define BLINK_PERIOD_MULTIPLIER 15

#define declare_led_fields(name) \
	uint8_t name ## Target, name ## Count, name ## PulseValue, name ## Increment

typedef struct {
#ifdef LED_READY_ENABLED
	declare_led_fields(Ready);
#endif
#ifdef LED_ERROR_ENABLED
	declare_led_fields(Error);
#endif
#ifdef LED_DATA_ENABLED
	declare_led_fields(Data);
#endif
	TIME_T last;
	uint8_t multiplier;
} LEDData_t;

LEDData_t LED;

#define MIN(a,b) ( ((a) < (b) ) ? (a) : (b) )
#define MAX(a,b) ( ((a) > (b) ) ? (a) : (b) )
#define CLAMP(value,min,max) MIN(MAX(min,value),max)

#define CAT(a,b) PRIMITIVE_CAT(a,b)
#define PRIMITIVE_CAT(a,b) a ## b

#define is_io_on(port,pin) ( (CAT(PORT,port) & _BV(pin)) != 0x00 )
#define io_on(port,pin) CAT(PORT,port) |= _BV(pin)
#define io_off(port,pin) CAT(PORT,port) &= ~(_BV(pin))
#define io_toggle(port,pin) CAT(PORT,port) ^= _BV(pin)
#define io_state(port,pin) (CAT(PORT,port) & _BV(pin))



#ifdef LED_PULSE_ENABLED

#define enable_comp(timer,comp) do{	  \
		CAT(CAT(TCCR,timer),A) |= _BV(CAT(CAT(COM,comp),1)); \
	}while(0)
#define disable_comp(timer,comp) do{\
		CAT(CAT(TCCR,timer),A) &= ~(_BV(CAT(CAT(COM,comp),1))); \
	}while(0)

#define implements_led_io(name,port,pin,timer,comp)	  \
	void LED ## name ## SetBrightness(uint8_t b) { \
		if (b == 0x00) { \
			disable_comp(timer,comp); \
			io_off(port,pin); \
			return; \
		} \
		uint8_t sreg = SREG; \
		cli(); \
		CAT(CAT(OCR,timer),comp) = b; \
		enable_comp(timer,comp); \
		SREG = sreg; \
	} \
	void LED## name ##On() { \
		disable_comp(timer,comp); \
		LED.name ## Target = 0; \
		io_on(port,pin); \
	} \
	void LED## name ## Off() { \
		disable_comp(timer,comp); \
		LED.name ## Target = 0; \
		io_off(port,pin); \
	} \
	void LED## name ## Toggle() { \
		if(LED.name ## Target == 0) { \
			disable_comp(timer,comp); \
			io_toggle(port,pin); \
		} else { \
			LED ## name ## Off(); \
		} \
	} \
	void LED ## name ## Blink( uint8_t count) { \
		disable_comp(timer,comp); \
		count = CLAMP(MAX(count,LED.name ## Target),1,8); \
		if (LED.name ## Target == 0 ) { \
			LED.name ## Count = count * 2; \
		} \
		LED.name ## Target = count; \
}	  \
	void LED ## name ## Pulse() { \
		if ( is_io_on(port,pin) ){ \
			LED.name ## PulseValue = 0xff; \
		} else { \
			LED.name ## PulseValue = 0x00; \
		} \
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

#ifdef LED_READY_ENABLED
implements_led_io(Ready,LED_READY_PORT,LED_READY_PIN,LED_READY_TIMER,LED_READY_COMP)
#endif

#ifdef LED_ERROR_ENABLED
implements_led_io(Error,LED_ERROR_PORT,LED_ERROR_PIN,LED_ERROR_TIMER,LED_ERROR_COMP)
#endif

#ifdef LED_DATA_ENABLED
implements_led_io(Data,LED_DATA_PORT,LED_DATA_PIN,LED_DATA_TIMER,LED_DATA_COMP)
#endif

#else

#define implements_led_io(name,port,pin)	  \
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
		count = CLAMP(MAX(count,LED.name ## Target),1,8); \
		if (LED.name ## Target == 0 ) { \
			LED.name ## Count = count * 2; \
		} \
		LED.name ## Target = count; \
	} \
	void LED ## name ## Loop(bool blink) { \
		if (LED.name ## Target == 0) { \
			return; \
		} \
		if ( !blink ) { \
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
	}

#ifdef LED_READY_ENABLED
implements_led_io(Ready,LED_READY_PORT,LED_READY_PIN)
#endif

#ifdef LED_ERROR_ENABLED
implements_led_io(Error,LED_ERROR_PORT,LED_ERROR_PIN)
#endif

#ifdef LED_DATA_ENABLED
implements_led_io(Data,LED_DATA_PORT,LED_DATA_PIN)
#endif

#endif //defined(LED_PULSE_ENABLED)

#define init_led_io(name,port,pin) do {	  \
		CAT(DDR,port) |= _BV(pin); \
		LED ## name ## Off(); \
	}while(0)
#define init_timer(timer) do{ \
		CAT(CAT(TCCR,timer),A) = _BV(CAT(CAT(WGM,timer),0)); \
		CAT(CAT(TCCR,timer),B) = _BV(CAT(CAT(WGM,timer),2)) | _BV(CAT(CAT(CS,timer),0)); \
	}while(0)


void InitLEDs() {
#ifdef LED_READY_ENABLED
	init_led_io(Ready,LED_READY_PORT,LED_READY_PIN);
#ifdef LED_PULSE_ENABLED
	init_timer(LED_READY_TIMER);
#endif
#endif
#ifdef LED_ERROR_ENABLED
	init_led_io(Error,LED_ERROR_PORT,LED_ERROR_PIN);
#ifdef LED_PULSE_ENABLED
	init_timer(LED_ERROR_TIMER);
#endif
#endif
#ifdef LED_DATA_ENABLED
	init_led_io(Data,LED_DATA_PORT,LED_DATA_PIN);
#ifdef LED_PULSE_ENABLED
	init_timer(LED_DATA_TIMER);
#endif
#endif
	LED.last = 0;
}




void ProcessLEDs() {
	TIME_T now = GET_TIME();
	if ( (now-LED.last) <  PULSE_PERIOD_MS) {
		return;
	}
	LED.last = now;
	bool blink = false;
	if ( ++LED.multiplier == BLINK_PERIOD_MULTIPLIER ) {
		LED.multiplier = 0;
		blink = true;
	}

#ifdef LED_READY_ENABLED
	LEDReadyLoop(blink);
#endif
#ifdef LED_ERROR_ENABLED
	LEDErrorLoop(blink);
#endif
#ifdef LED_DATA_ENABLED
	LEDDataLoop(blink);
#endif

}

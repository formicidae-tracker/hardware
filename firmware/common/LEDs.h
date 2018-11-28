#pragma once

#include "inttypes.h"

#include "config.h"


#ifdef LED_PULSE_ENABLED

#define define_led_io(name)	  \
	void LED ## name ## On(); \
	void LED ## name ## Off(); \
	void LED ## name ## Toggle(); \
	void LED ## name ## Blink(uint8_t times); \
	void LED ## name ## Pulse()

#if defined(LED_READY_PORT) && defined(LED_READY_PIN) && defined(LED_READY_TIMER) && defined(LED_READY_COMP)
#define LED_READY_ENABLED
#endif

#if defined(LED_ERROR_PORT) && defined(LED_ERROR_PIN) && defined(LED_ERROR_TIMER) && defined(LED_ERROR_COMP)
#define LED_ERROR_ENABLED
#endif

#if defined(LED_DATA_PORT) && defined(LED_DATA_PIN) && defined(LED_DATA_TIMER) && defined(LED_DATA_COMP)
#define LED_DATA_ENABLED
#endif


#else //defined LED_PULSE_ENABLED

#define define_led_io(name)	  \
	void LED ## name ## On(); \
	void LED ## name ## Off(); \
	void LED ## name ## Toggle(); \
	void LED ## name ## Blink(uint8_t times)

#if defined(LED_READY_PORT) && defined(LED_READY_PIN)
#define LED_READY_ENABLED
#endif

#if defined(LED_ERROR_PORT) && defined(LED_ERROR_PIN)
#define LED_ERROR_ENABLED
#endif

#if defined(LED_DATA_PORT) && defined(LED_DATA_PIN)
#define LED_DATA_ENABLED
#endif

#endif //defined LED_PULSE_ENABLED

void InitLEDs();


#ifdef LED_READY_ENABLED
define_led_io(Ready);
#endif
#ifdef LED_ERROR_ENABLED
define_led_io(Error);
#endif
#ifdef LED_DATA_ENABLED
define_led_io(Data);
#endif

void ProcessLEDs();

#pragma once

#include "inttypes.h"

#define define_led_io(name)	  \
	void LED ## name ## On(); \
	void LED ## name ## Off(); \
	void LED ## name ## Toggle(); \
	void LED ## name ## Blink(uint8_t times); \
	void LED ## name ## Pulse() \

void InitLEDs();
define_led_io(Ready);
define_led_io(Error);
void ProcessLEDs();

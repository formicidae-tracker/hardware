#pragma once


#define define_led_io(name)	  \
	void LED ## name ## On(); \
	void LED ## name ## Off(); \
	void LED ## name ## Toggle() \

void InitLEDs();
define_led_io(Ready);
define_led_io(Error);
define_led_io(Data);

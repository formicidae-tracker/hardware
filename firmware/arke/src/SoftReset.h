#pragma once

#include <avr/wdt.h>

#define software_reset() do {\
		wdt_enable(WDTO_15MS); \
		for(;;) { } \
	}while(0)

void wdt_init(void) __attribute__((naked)) __attribute__((section(".init3")));

#include <stdbool.h>

//#include "Debug.h"
#include "LightManager.h"
//#include "ChargeMonitor.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <inttypes.h>
#include <util/delay.h>

void display(uint8_t data) {
	uint8_t toDisplay = data << 4;
	PORTA &= (~0x30) | toDisplay;
	PORTA |= 0x30 & toDisplay;
}



int main() {


	//	InitDebug();
	//	InitChargeMonitor();
	InitLightManager();
	//	Log(0);
	DDRA |= _BV(5) | _BV(4);

	uint8_t seconds = 1;
	display(seconds);
	uint16_t start = LMSystime();
	while(true) {

		uint16_t now = LMSystime();
		if ( now - start >= 610 ) {
			display(++seconds);
			start = now;
		}
		//DProcess();
	}

	return 0;
}

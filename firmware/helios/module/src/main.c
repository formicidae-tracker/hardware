#include <stdbool.h>


#include "Systime.h"
#include "ChargeMonitor.h"
#include "LightManager.h"
#include "SerialInterface.h"
#include <avr/io.h>


#define SET_ENABLED() PORTB |= _BV(5);
#define CLEAR_ENABLED() PORTB &= ~(_BV(5));
#define IS_ENABLED() (PORTB & _BV(5) )

int main() {
	//	InitSystime();

	InitChargeMonitor();
	InitLightManager();
	InitSerialInterface();


	DDRB |= _BV(4) | _BV(5);
	LMSetBrightness(IR,0);
	LMSetBrightness(VISIBLE,0);
	LMSetBrightness(UV,0);
	/* Systime_t last = 0; */
	/* uint8_t value =0; */
	/* int8_t incr =1; */
	while(true) {
		if ( CMCheckCharge() == true ) {
			if ( IS_ENABLED() == 0x00 ) {
				LMActivateOutput();
				SET_ENABLED();
			}
		} else if ( IS_ENABLED() != 0 ){
			LMDeactivateOutput();
			CLEAR_ENABLED();
		}
		/* Systime_t now = GetSystime(); */

		/* if ((now - last) >= 25 ) { */
		/* 	last = now; */
		/* 	if (value ==  255) { */
		/* 		incr = -1; */
		/* 	} else if (value == 0) { */
		/* 		incr = 1; */
		/* 	} */
		/* 	value += incr; */
		/* 	LMSetBrightness(VISIBLE,value); */

		/* } */

		SIProcess();

	}

	return 0;
}

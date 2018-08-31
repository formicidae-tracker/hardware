#include <stdbool.h>


#include "Systime.h"
#include "ChargeMonitor.h"
#include "LightManager.h"
#include "SerialInterface.h"
#include <avr/io.h>


#define SET_ENABLED() PORTB |= _BV(4);
#define CLEAR_ENABLED() PORTB &= ~(_BV(4));
#define IS_ENABLED() (PORTB & _BV(4) )

int main() {
	InitSystime();

	InitChargeMonitor();
	InitLightManager();
	InitSerialInterface();


	DDRB |= _BV(4);
	LMSetBrightness(IR,0);
	LMSetBrightness(VISIBLE,0);
	LMSetBrightness(UV,0);

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

		SIProcess();

	}

	return 0;
}

#include <stdbool.h>


#include "Systime.h"
#include "ChargeMonitor.h"
#include "LightManager.h"

#include <avr/io.h>

#define VIS_LEVEL 255

#define SET_ENABLED() PORTB |= _BV(4);
#define CLEAR_ENABLED() PORTB &= ~(_BV(4));
#define IS_ENABLED() (PORTB & _BV(4) )

int main() {
	InitSystime();

	InitChargeMonitor();
	InitLightManager();

	DDRB |= _BV(4);
	Systime_t lastIR=0;
	Systime_t lastVisible=0;
	LMSetBrightness(IR,0);
	LMSetBrightness(VISIBLE,0);
	uint8_t visibleBrightness = 0;
	while(true) {
		if ( CMCheckCharge() == true ) {
			if ( IS_ENABLED() == 0x00 ) {
				LMActivateOutput();
				SET_ENABLED();
				lastIR = GetSystime();
				lastVisible = lastIR;
			}
		} else if ( IS_ENABLED() != 0 ){
			LMDeactivateOutput();
			CLEAR_ENABLED();
		}

		if(  IS_ENABLED() == 0x00 ) {
			continue;
		}

		Systime_t now = GetSystime();

		if ( (now - lastIR) >= 28 ) {
			LMStartPulse();
			lastIR = now;
		}

		if ( (visibleBrightness != VIS_LEVEL) && ( (now - lastVisible) >= 25 ) ) {
			LMSetBrightness(VISIBLE,++visibleBrightness);
			lastVisible = now;
		}

	}

	return 0;
}

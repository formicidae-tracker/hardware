#include <stdbool.h>

#include "Debug.h"
#include "LightManager.h"
#include "ChargeMonitor.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <inttypes.h>
#include <util/delay.h>

int main() {


	//	InitDebug();
	InitChargeMonitor();
	InitLightManager();

	//Log(0xaa);
	DDRA |= _BV(5);



	uint16_t lastVisible = 0;
	uint16_t lastIR = 0;
	uint8_t value = 0;
	bool first = true;
	bool isOn = false;
	LMSetBrightness(VISIBLE,value);
	while(true) {
		if (CMCheckCharge() == true ) {
			isOn = true;
			if ( first == true ) {
				LMActivateOutput();
				lastVisible = LMSystime();
				lastIR = lastVisible;
				first =false;
			}
		} else  {
			value = 0;
			LMDeactivateOutput();
			first = true;
			isOn = false;
		}
		//		DProcess();
		uint16_t now = LMSystime();
		if ( isOn && ((now-lastVisible) > 12) && (value != 255)) {
			++value;
			LMSetBrightness(VISIBLE,value);
			lastVisible = now;
		}

		if ( ((now-lastIR) >= 18) && isOn ) {
			PORTA ^= _BV(5);
			LMStartPulse();
			lastIR = now;
		}


	}

	return 0;
}

#include <stdbool.h>

#include "Debug.h"
#include "LightManager.h"
#include "ChargeMonitor.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <inttypes.h>
#include <util/delay.h>

int main() {


	InitDebug();
	InitChargeMonitor();
	InitLightManager();

	//Log(0xaa);

	uint16_t last = 0;
	uint8_t value = 0;
	bool first = true;
	LMSetBrightness(VISIBLE,value);
	while(true) {
		if (CMCheckCharge() == true ) {
			if ( first == true ) {
				LMActivateOutput();
				last = LMSystime();
				first =false;
			}
		} else  {
			value = 0;
			LMDeactivateOutput();
			first = true;
		}
		//		DProcess();
		uint16_t now = LMSystime();
		if ( ((now-last) > 12) && (value != 255)) {
			LMSetBrightness(VISIBLE,++value);
			last = now;
		}




	}

	return 0;
}

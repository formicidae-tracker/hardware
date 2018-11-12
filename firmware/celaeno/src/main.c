#include <arke-avr/systime.h>

#include "LEDs.h"
#include <arke-avr.h>
#include "FanControl.h"



#define NOMINAL_LVL 0x30

implements_ArkeSoftwareReset()

int main() {
	InitLEDs();
	LEDReadyPulse();
	LEDErrorOff();
	DDRD |= _BV(0) ;

	InitFanControl();
	ArkeSystime_t last = -10000;

	InitArke();

	uint8_t i = 1;

	while(true) {
		ProcessLEDs();
		ArkeProcess();
		FanControlStatus_e s = ProcessFanControl();
		if ( (s & FAN_1_STALL) != 0x00 ) {
			LEDErrorOn();
		} else {
			LEDErrorOff();
		}
		ArkeSystime_t now = ArkeGetSystime();
		if ( (now-last) >= 10000 ) {
			if (++i % 2 == 1) {
				SetFan1Power(0x00);
				SetFan2Power(NOMINAL_LVL);
			} else {
				SetFan1Power(NOMINAL_LVL);
				SetFan2Power(0);
			}
			last = now;
		}
	}
}

#include <arke-avr.h>

#include "LEDs.h"
#include "Systime.h"
#include "FanControl.h"

#include <yaacl.h>
#define NOMINAL_LVL 0x30
int main() {
	InitSystime();
	InitLEDs();
	LEDReadyPulse();
	LEDErrorOff();
	DDRD |= _BV(0) ;

	InitFanControl();
	Systime_t last = 0;
	uint8_t i = 1;

	while(true) {
		ProcessLEDs();
		FanControlStatus_e s = ProcessFanControl();
		if ( (s & FAN_1_STALL) != 0x00 ) {
			LEDErrorOn();
		} else {
			LEDErrorOff();
		}
		Systime_t now = GetSystime();
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

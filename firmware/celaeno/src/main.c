#include <arke-avr.h>

#include "LEDs.h"
#include "Systime.h"
#include "FanControl.h"

#include <yaacl.h>

int main() {
	InitSystime();
	InitLEDs();
	LEDReadyPulse();
	LEDErrorOff();
	DDRD |= _BV(0) ;

	InitFanControl();
	SetFan1Power(0x80);
	Systime_t last = 0;
	uint8_t i = 0;
	yaacl_config_t c;
	c.baudrate = YAACL_BR_200;
	yaacl_init(&c);

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
				SetFan2Power(0x80);
			} else {
				SetFan1Power(0x80);
				SetFan2Power(0x00);
			}
			last = now;
		}
	}
}

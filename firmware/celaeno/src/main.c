#include <arke-avr.h>

#include "LEDs.h"
#include "Systime.h"

#include <yaacl.h>
#include <yaail.h>


int main() {
	InitSystime();
	InitLEDs();
	LEDReadyPulse();
	LEDErrorOff();
	Systime_t last = -5000;
	DDRD |= _BV(0) ;
	yaail_init(YAAIL_100);

	uint8_t data[2];
	while(true) {
		ProcessLEDs();
		switch(yaail_poll()) {
		case YAAIL_DONE:
			yaail_rearm();
			LEDErrorOff();
			break;
		case YAAIL_BUSY:
		case YAAIL_NO_ERROR:
			break;
		case YAAIL_START_ERROR:
			yaail_rearm();
			LEDErrorBlink(2);
			break;
		case YAAIL_ACK_ERROR:
			yaail_rearm();
			//LEDErrorBlink(3);
			break;
		default:
			break;
		}

		Systime_t now = GetSystime();
		if ( (now-last) >= 100  ) {
			last = now;
			if (yaail_poll() == YAAIL_NO_ERROR) {
				data[0] = 0x3c;
				yaail_write_and_read(0x2e,data,1,2);
			}
		}
	}
}

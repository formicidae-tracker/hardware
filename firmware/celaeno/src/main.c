#include <arke-avr.h>

#include "LEDs.h"
#include "Systime.h"
#include <util/delay.h>


#include <yaacl.h>
#include <yaail.h>

uint16_t lookup[512];


int main() {
	InitSystime();
	InitLEDs();
	LEDReadyPulse();
	LEDErrorOff();
	DDRD |= _BV(0) ;
	yaail_init(YAAIL_100);

	uint8_t data[2];
	data[0] = 0x33;
	data[1] = 0x68;
	yaail_write(0x2e,data,2);
	while( YAAIL_BUSY == yaail_poll() ){
	}
	yaail_rearm();
	data[0] = 0x32;
	data[1] = 0x03;
	yaail_write(0x2e,data,2);
	while( YAAIL_BUSY == yaail_poll() ){
	}
	yaail_rearm();
	data[0] = 0x30;
	data[1] = 0x30;
	yaail_write(0x2e,data,2);
	while( YAAIL_BUSY == yaail_poll() ){
	}
	yaail_rearm();
	data[0] = 0x20;
	data[1] = 0x40;
	yaail_write(0x2e,data,2);
	while( YAAIL_BUSY == yaail_poll() ){
	}
	yaail_rearm();

	uint8_t i= 0;
	while(true) {
		ProcessLEDs();

		switch(yaail_poll()) {
		case YAAIL_NO_ERROR:
			_delay_ms(1);
			data[0] = 0x3e | ( ++i & 0x01 ) ;
			yaail_write_and_read(0x2e,data,1,1);
			break;
		case YAAIL_BUSY:
			break;
		case YAAIL_ACK_ERROR:
		case YAAIL_DONE:
			yaail_write(lookup[data[0]],0,0);
			LEDErrorOff();
			yaail_rearm();
			break;
		default:
			LEDErrorOn();
			yaail_rearm();
		}

	}
}

#include <stdbool.h>


#include "Systime.h"
#include "ChargeMonitor.h"
#include "LightManager.h"
#include "SerialInterface.h"
#include <avr/io.h>


#define SET_ENABLED() PORTB |= _BV(5);
#define CLEAR_ENABLED() PORTB &= ~(_BV(5));
#define IS_ENABLED() (PORTB & _BV(5) )

typedef enum {
	IDLE = 0,
	IR_PULSING = 1,
	VISIBLE_ON = 2,
	VISIBLE_OFF = 3,
	UV_ON = 4,
	UV_OFF = 5,
} Stage_t;

int main() {
	InitSystime();

	InitChargeMonitor();
	InitLightManager();
	//InitSerialInterface();


	DDRB |= _BV(4) | _BV(5);
	LMSetBrightness(IR,0);
	LMSetBrightness(VISIBLE,0);
	LMSetBrightness(UV,0);
	Systime_t last = 0;
	Stage_t stage = IDLE;
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

		if ( IS_ENABLED() == 0 ) {
			continue;
		}


		if ( stage == IDLE ) {
			LMStartPulse();
			stage = IR_PULSING;
		}

		Systime_t now = GetSystime();
		if ( (now % 2) == 0 ) {
			PORTB |= _BV(4);
		} else {
			PORTB &= ~_BV(4);
		}

		if ( (now - last) >= 1000 && stage == IR_PULSING ) {
			LMSetBrightness(VISIBLE,255);
			stage = VISIBLE_ON;
		}

		if ( ( now - last ) >= 1100 && stage == VISIBLE_ON ) {
			LMSetBrightness(VISIBLE,0);
			stage = VISIBLE_OFF;
		}

		if ( (now - last ) >= 2000 && stage == VISIBLE_OFF) {
			LMSetBrightness(UV,255);
			stage = UV_ON;
		}

		if ( (now - last) >= 2100 && stage == UV_ON ) {
			LMSetBrightness(UV,0);
			stage = UV_OFF;
		}

		if ( (now - last) >= 3000 && stage == UV_OFF ) {
			stage = IDLE;
			last = now;
		}


		// SIProcess();

	}

	return 0;
}

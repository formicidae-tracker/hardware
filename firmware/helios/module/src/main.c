#include <stdbool.h>


#include "Systime.h"
#include "ChargeMonitor.h"
#include "LightManager.h"
#include "SerialInterface.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <avr/eeprom.h>

#define SET_ENABLED() PORTB |= _BV(5);
#define CLEAR_ENABLED() PORTB &= ~(_BV(5));
#define IS_ENABLED() (PORTB & _BV(5) )

uint8_t EEMEM EEVisible = 0;
uint8_t EEMEM EEUV = 0;


int main() {
	InitSystime();

	InitChargeMonitor();
	InitLightManager();
	InitSerialInterface();

	DDRB |= _BV(4) | _BV(5);
	uint8_t visible = eeprom_read_byte(&EEVisible);
	uint8_t uv = eeprom_read_byte(&EEUV);

	LMSetBrightness(IR,0);
	LMSetBrightness(VISIBLE,visible);
	LMSetBrightness(UV,uv);
	uint8_t changed = 0;
	Systime_t lastChange = 0;

	wdt_enable(WDTO_15MS);
	while(true) {
		wdt_reset();
		if ( XFDCSR & _BV(XFDIF) ) {
			//external clock failure, we issue a software reset
			// by letting go the watchdog;
			for (;;) {}
		}

		if ( CMCheckCharge() == true ) {
			if ( IS_ENABLED() == 0x00 ) {
				LMActivateOutput();
				SET_ENABLED();
			}
		} else if ( IS_ENABLED() != 0 ){
			LMDeactivateOutput();
			CLEAR_ENABLED();
		}
		uint8_t oldVisible = visible;
		uint8_t oldUV = uv;

		Systime_t now = GetSystime();
		if ( SIProcess(&visible,&uv) !=  0 ) {
			LMSetBrightness(VISIBLE,visible);
			LMSetBrightness(UV,uv);
			if (oldVisible != visible || oldUV != uv) {
				changed = 1;
				lastChange = now;
			}
		}

		if ( changed != 0 && (now - lastChange) >= 60000 ) {
			changed = 0;
			uint8_t sreg = SREG;
			cli();
			eeprom_update_byte(&EEVisible,visible);
			eeprom_update_byte(&EEUV,uv);
			SREG = sreg;
		}

	}

	return 0;
}

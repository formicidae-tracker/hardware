#include "Debug.h"

#include <avr/io.h>

#include "LightManager.h"


typedef struct {
	uint8_t value;
	uint8_t error;
	uint8_t displayed;
	uint16_t lastDisplayed;
} DebugData_t;

DebugData_t debug;

void Ddisplay(uint8_t data) {
	uint8_t toDisplay = data << 4;
	PORTA &= (~0x30) | toDisplay;
	PORTA |= 0x30 & toDisplay;
}

//Bit 0 : A5
//Bit 1 : A4
void InitDebug() {
	DDRA |= _BV(5) | _BV(4);
	Ddisplay(0);
	debug.value = 0;
	debug.error = 0;
	debug.displayed = 0;
	debug.lastDisplayed = 0;
}

void Log(uint8_t data) {
	debug.value = data;
}

void ReportError(uint8_t error) {
	debug.error = error;
}

void DProcess() {
	uint16_t now = LMSystime();
	if ( ( now - debug.lastDisplayed ) < 305 ) {
		return;
	}
	debug.lastDisplayed = now;

	++debug.displayed;
	if (debug.error == 0) {
		if (debug.displayed >= 4) {
			Ddisplay(0);
			debug.displayed = 0xff;
			return;
		}
		Ddisplay(debug.value >> (2*debug.displayed));
		return;
	}

	if ( (debug.displayed & 0x01) == 1) {
		Ddisplay(0);

		return;
	}
	if(debug.displayed < 8) {
		Ddisplay(debug.error >> debug.displayed);
	} else {
		debug.displayed = 0xff;
	}

}

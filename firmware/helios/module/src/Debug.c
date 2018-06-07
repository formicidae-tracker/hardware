#include "Debug.h"

#include <avr/io.h>

#include "LightManager.h"


typedef struct {
	uint8_t value;
	uint8_t error;
	uint8_t displayed;
} DebugData_t;

DebugData_t debug;

//Bit 0 : A5
//Bit 1 : A4

void InitDebug() {
	DDRA |= _BV(5) | _BV(4);
	debug.value = 0;
	debug.error = 0;
	debug.displayed = 0;
}

void Log(uint8_t data) {
	debug.value = data;
}

void ReportError(uint8_t error) {
	debug.error = error;
}

void DProcess() {
	if ( (LMSystime() & 0x7f) != 0 ) {
		return;
	}
	debug.displayed = (debug.displayed + 1) & 0x07;
	if (debug.error == 0) {
		if (debug.displayed >= 4) {
			PORTA &= ~(_BV(5) | _BV(4) ) ;
			return;
		}
		uint8_t toPrint = ((debug.value >> debug.displayed) & 0x03) << 4;
		PORTA &= toPrint || ~0x30;
		PORTA |= toPrint;
		return;
	}

if ( (debug.displayed & 0x01) == 1) {
		PORTA &= ~(_BV(5) | _BV(4) ) ;
		return;
	}

	uint8_t toPrint = ((debug.value >> (debug.displayed >> 1) ) & 0x03) << 4;
	PORTA &= toPrint || ~0x30;
	PORTA |= toPrint;


}

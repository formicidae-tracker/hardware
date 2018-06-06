#include "Debug.h"

#include <avr/eeprom.h>

#define EEPROM_SIZE 128

#define increment_address(a) do {\
		a = (uint8_t*)(((int)a + 1) & (EEPROM_SIZE-1)); \
	} while(0)

typedef struct {
	uint8_t* address;
} DebugData_t;

DebugData_t debug;

void InitDebug() {
	debug.address = 0;
	while ( (eeprom_read_byte(debug.address) != 0) && (debug.address < (uint8_t*)(EEPROM_SIZE-1)) ) {
		increment_address(debug.address);
	}
}

void Log(uint8_t data) {
	eeprom_write_byte(debug.address,(data & 0x3f) | 0x80);
	increment_address(debug.address);
}

void ReportError(uint8_t error) {
	eeprom_write_byte(debug.address,error | 0xc0);
	increment_address(debug.address);
}

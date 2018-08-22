#include "ModuleManager.h"

#include <avr/io.h>

#define MESSAGE_LENGTH 5
#define MESSAGE_START 0xaa

#define IR_POS 1
#define VIS_POS 2
#define UV_POS 3
#define CS_POS 4


typedef struct {
	uint8_t buffer[MESSAGE_LENGTH];
	uint8_t pos;

} ModuleManagerData_t;

ModuleManagerData_t MM;

void InitModuleManager() {
	MM.pos = MESSAGE_LENGTH;
	MM.buffer[0] = MESSAGE_START;

	// enable UART in TX only mode
	LINCR = _BV(LENA) |  _BV(LCMD2) | _BV(LCMD0);

	// set baudrate to 114285 within 0.79% of 115200
	// BAUD = FIO / ( LBT * (LDIV + 1) )
	// LBT = 35 LDIV = 3
	LINBTR = 35;
	LINBRR = 3;


}

void ProcessModuleManager() {
	if ( ( LINSIR & _BV(LTXOK) ) == 0x00) {
		return;
	}


	// check if we are done sending the message
	if (MM.pos ==  MESSAGE_LENGTH ) {
		return;
	}

	//inrement byte to send
	++MM.pos;

	// done again ?
	if (MM.pos == MESSAGE_LENGTH ) {
		return;
	}

	//send next byte
	LINDAT = MM.buffer[MM.pos];

}

void SendToModule(uint8_t ir, uint8_t vis, uint8_t uv) {
	if (MM.pos != MESSAGE_LENGTH ) {
		return;
	}

	//compute message
	MM.buffer[IR_POS] = ir;
	MM.buffer[VIS_POS] = vis;
	MM.buffer[UV_POS] = uv;
	MM.buffer[CS_POS] = ( (MESSAGE_START ^ ir) ^ vis ) ^ uv;

	//send first byte
	MM.pos = 0;
	LINDAT = MM.buffer[0];

}

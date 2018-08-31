#include "SerialInterface.h"

#include <avr/io.h>
#include <avr/interrupt.h>

#include "common/serial_protocol.h"

#include "LightManager.h"

typedef struct {
	uint8_t received;
	uint8_t buffer[MESSAGE_LENGTH];

} SerialInterface_t;

SerialInterface_t SI;

void InitSerialInterface() {
	uint8_t sreg = SREG;
	cli();
	// from a 20MHz clocks, sets the baudrate to 115200
	UBRR0 = 10;
	//RX mode only
	UCSR0B = _BV(RXEN0);
	UCSR0C = _BV(UCSZ11) | _BV(UCSZ10);

	SI.received = 0;

	SREG = sreg;
}

void SIProcess() {

	if ( (UCSR0A & _BV(RXC0) ) == 0 ) {
		// no char received yet
		return;
	}

	uint8_t data = UDR0;

	if (SI.received == 0 ) {
		//waiting for start character
		if (data != MESSAGE_START ) {
			return;
		}
		++SI.received;
		return;
	}
	// data char
	SI.buffer[SI.received] = data;
	++SI.received;

	if (SI.received != MESSAGE_LENGTH) {
		//not enough char received, wait for more
		return;
	}
	//we will treat the message, restart the RX loop now
	SI.received = 0;

	uint8_t cs = COMPUTE_CHECKSUM(SI.buffer);
	if (cs != SI.buffer[CS_POS]) {
		//checksum error
		return;
	}

	LMSetBrightness(VISIBLE, SI.buffer[VIS_POS]);
	LMSetBrightness(UV, SI.buffer[UV_POS]);
}

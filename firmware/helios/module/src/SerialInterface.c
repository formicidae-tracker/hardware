#include "SerialInterface.h"

#include <avr/io.h>
#include <avr/interrupt.h>

#include "common/serial_protocol.h"

typedef struct {
	uint8_t received;
	uint8_t buffer[MESSAGE_LENGTH];
} SerialInterface_t;

SerialInterface_t SI;



void InitSerialInterface() {
	uint8_t sreg = SREG;
	cli();
	// from a 20MHz clocks, sets the baudrate to 250000
	// BR = Fosc/(16*(UBRN+1)
	UBRR0 = 4;
	//RX mode only
	UCSR0C |= _BV(UCSZ01) | _BV(UCSZ00);

	UCSR0B |= _BV(RXEN0);

	SI.received = 0;

	SREG = sreg;

	DDRB |= _BV(4);
}

#define SET_ERROR() PORTB |= _BV(4)
#define CLEAR_ERROR() PORTB &= ~(_BV(4))
#define TOGGLE_ERROR() PORTB ^= _BV(4)

uint8_t SIProcess(uint8_t * visible, uint8_t * UV) {
	if ( (UCSR0A & _BV(RXC0) ) == 0 ) {
		// no char received yet
		return 0;
	}

	uint8_t data = UDR0;

	if (SI.received == 0 ) {
		//waiting for start character
		if (data != MESSAGE_START ) {
			return 0;
		}
		++SI.received;
		return 0;
	}
	// data char
	SI.buffer[SI.received] = data;
	++SI.received;

	if (SI.received != MESSAGE_LENGTH) {
		//not enough char received, wait for more
		return 0;
	}
	//we will treat the message, restart the RX loop now
	SI.received = 0;

	uint8_t cs = COMPUTE_CHECKSUM(SI.buffer);
	if (cs != SI.buffer[CS_POS]) {
		TOGGLE_ERROR();
		//checksum error
		return 0;
	}
	*visible = SI.buffer[VIS_POS];
	*UV = SI.buffer[UV_POS];
	return 2;
}

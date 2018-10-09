#include "HostLink.h"

#include <avr/io.h>

#define RB_SIZE 32
#define RB_MASK (RB_SIZE-1)


typedef struct HostPacket {
	uint32_t ID;
	uint8_t length;
	uint8_t data[8];
} HostPacket;

#define START_OF_FRAME_SIZE 2
#define START_OF_FRAME_BYTE 0xaa
#define HOST_PACKET_SIZE(packet) (START_OF_FRAME_SIZE + 5 +  (packet).length)
#define HOST_PACKET_HEADER_SIZE HOST_PACKET_SIZE(0)
#define CHANNEL_IS_FREE(ch) (ch == 0xff)
#define CHANNEL_MARK_FREE(ch) ch = 0xff;

typedef struct HostPacketRingBuffer {
	HostPacket data[RB_SIZE];
	uint8_t head;
	uint8_t tail;
	uint8_t size;
} HostPacketRingBuffer;


typedef struct HostData {
	HostPacketRingBuffer in,out;
	uint8_t sent,received;
} HostData;
HostData link;


int HostSendControlPacket(uint8_t code, uint8_t * buffer);

#define RB_INIT(rb) do {	  \
		(rb).head = 0; \
		(rb).tail = 0; \
		(rb).size = 0; \
	}while(0)
#define RB_HEAD(rb) ((rb).data[(rb).head])
#define RB_TAIL(rb) ((rb).data[(rb).tail])
#define RB_EMPTY(rb) ((rb).size == 0)
#define RB_FULL(rb) ((rb).size >= RB_SIZE)
#define RB_INCREMENT_TAIL(rb) do { \
		++(rb).size; \
		(rb).tail = ((rb).tail + 1) & RB_MASK; \
	}while(0)

#define RB_INCREMENT_HEAD(rb) do { \
		--(rb).size; \
		(rb).head = ((rb).head + 1) & RB_MASK; \
	}while(0)





// Init the UART link.
void InitHostLink() {
	RB_INIT(link.in);
	RB_INIT(link.out);

	CHANNEL_MARK_FREE(link.sent);
	CHANNEL_MARK_FREE(link.received);



	// enable UART in TX only mode
	LINCR = _BV(LENA) |  _BV(LCMD2) | _BV(LCMD0);

	// set baudrate to 114285 within 0.79% of 115200
	// BAUD = FIO / ( LBT * (LDIV + 1) )
	// LBT = 35 LDIV = 3
	LINBTR = 32;
	LINBRR = 25;

}

void ProcessHostTx() {
	if ( (LINSIR & _BV(LTXOK) ) == 0x00 ) {
		//UART busy so nothing to do
		return;
	}

	if (RB_EMPTY(link.out) ) {
		// not data to send out
		return;
	}

	if (CHANNEL_IS_FREE(link.sent) ) {
		// we start a new communication
		link.sent = 0;
		LINDAT = START_OF_FRAME_BYTE;
		return;
	}
	++link.sent;

	if ( link.sent >= HOST_PACKET_SIZE(RB_HEAD(link.out)) ) {
		CHANNEL_MARK_FREE(link.sent);
		RB_INCREMENT_HEAD(link.out);
		return;
	}

	if (link.sent < START_OF_FRAME_SIZE ) {
		LINDAT = START_OF_FRAME_BYTE;
		return;
	}

	LINDAT = RB_HEAD(link.out).data[link.sent-START_OF_FRAME_BYTE];

}

void ProcessHostRx() {
	if ( (LINSIR & _BV(LTXOK) ) == 0x00 ) {
		//UART still waiting for  a char
		return;
	}

	if ( CHANNEL_IS_FREE(link.received) ) {
		if ( RB_FULL(link.in) ) {
			//TODO: report overflow error condition ?
			return ;
		}
		link.received = 0;
	}

	uint8_t data = LINDAT;
	++link.received;
	if (link.received <= START_OF_FRAME_SIZE ) {
		//lokking for start of frame
		if( data != START_OF_FRAME_BYTE ) {
			// not the  case, we are waiting for a new frame
			CHANNEL_MARK_FREE(link.received);
			//TODO:  report error condition ?
		}
		return;
	}
	HostPacket * incoming = &RB_TAIL(link.in);
	uint8_t * incomingAsBuffer = (uint8_t*)incoming;
	uint8_t ptrOffset = link.received - START_OF_FRAME_SIZE - 1;
	incomingAsBuffer[ptrOffset] = data;
	if (link.received == (START_OF_FRAME_SIZE + 1) ) {
		//resets incommin length
		incoming->length = 0x00;
	}

	if (link.received >= HOST_PACKET_SIZE(*incoming) ) {
		RB_INCREMENT_TAIL(link.in);
	}

}


void ProcessIncoming() {
	if ( RB_EMPTY(link.in) ) {
		return;
	}
	//TODO :  Incoming logic process


	RB_INCREMENT_HEAD(link.in);
}

// Process incoming packet from host
void ProcessHostLink() {
	ProcessHostTx();
	ProcessHostRx();
	ProcessIncoming();
}


int HostSendControlPacket(uint8_t code, uint8_t * buffer) {
	if(RB_FULL(link.out)) {
		// TODO report error
		return 1;
	}
	HostPacket * data = &RB_TAIL(link.out);
	data->ID = 0xffffffff;
	data->length = code;
	for (uint8_t i = 0; i<8; ++i) {
		data->data[i]  = buffer[i];
	}

	RB_INCREMENT_TAIL(link.out);
	return 0;
}

// Prepares an upstreamn
int HostSendCANPacket(const yaacl_txn_t * txn) {
	if(RB_FULL(link.out)) {
		// TODO report error
		return 1;
	}
	HostPacket * data = &RB_TAIL(link.out);
	if (txn->ID.std.rb0 ) {
		data->ID = txn->ID.ext.ID;
	} else {
		data->ID = txn->ID.std.ID;
	}
	//TODO  report RTR & IDE

	data->length = txn->length;
	for (uint8_t i = 0; i < txn->length; ++i ) {
		data->data[i] = txn->data[i];
	}

	RB_INCREMENT_TAIL(link.out);

	return 0;
}

#include "HostLink.h"

#include "yaacl.h"
#include "SoftReset.h"


#include <HostCommunication.h>

#include <avr/io.h>

#define RB_IN_SIZE 16
#define RB_OUT_SIZE 16
#define RB_MASK(size) (size-1)

#define MAX(a,b) ( ((a) > (b)) ? (a) : (b) )
#define MIN(a,b) ( ((a) < (b)) ? (a) : (b) )



#define CHANNEL_IS_FREE(ch) (ch == 0xff)
#define CHANNEL_MARK_FREE(ch) ch = 0xff;

typedef struct HostPacketInRingBuffer {
	HostPacket data[RB_IN_SIZE];
	uint8_t head;
	uint8_t tail;
	uint8_t size;
} HostPacketInRingBuffer;

typedef struct HostPacketOutRingBuffer {
	HostPacket data[RB_OUT_SIZE];
	uint8_t head;
	uint8_t tail;
	uint8_t size;
} HostPacketOutRingBuffer;

#define RB_INIT(rb) do {	  \
		(rb).head = 0; \
		(rb).tail = 0; \
		(rb).size = 0; \
	}while(0)
#define RB_HEAD(rb) ((rb).data[(rb).head])
#define RB_TAIL(rb) ((rb).data[(rb).tail])
#define RB_EMPTY(rb) ((rb).size == 0)
#define RB_FULL(rb,size_) ((rb).size >= size_)
#define RB_INCREMENT_TAIL(rb,size_) do {	  \
		++(rb).size; \
		(rb).tail = ((rb).tail + 1) & RB_MASK(size_); \
	}while(0)

#define RB_INCREMENT_HEAD(rb,size_) do {	  \
		--(rb).size; \
		(rb).head = ((rb).head + 1) & RB_MASK(size_); \
	}while(0)


typedef struct HostData {
	HostPacketInRingBuffer in;
	HostPacketOutRingBuffer out;
	uint8_t sent,received;
	yaacl_txn_t txn;
} HostData;
HostData link;



// Init the UART link.
void InitHostLink() {
	RB_INIT(link.in);
	RB_INIT(link.out);

	CHANNEL_MARK_FREE(link.sent);
	CHANNEL_MARK_FREE(link.received);
	yaacl_init_txn(&link.txn);

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
	uint8_t needed = HOST_PACKET_SIZE(RB_HEAD(link.out));

	if ( link.sent >= needed ) {
		CHANNEL_MARK_FREE(link.sent);
		RB_INCREMENT_HEAD(link.out,RB_OUT_SIZE);
		return;
	}

	if (link.sent < START_OF_FRAME_SIZE ) {
		LINDAT = START_OF_FRAME_BYTE;
		return;
	}

	if (link.sent == (needed -1) ) {
		//time to compute and send the checksum.
		uint8_t cs;
		arke_compute_checksum(RB_HEAD(link.out),cs);
		LINDAT = cs;
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
		if ( RB_FULL(link.in,RB_IN_SIZE) ) {
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
	if (link.received <= HEADER_SIZE ) {
		//we wait utinl we know the size
		incomingAsBuffer[ptrOffset] = data;
		return;
	}
	uint8_t needed = HOST_PACKET_SIZE(*incoming);
	if ( link.received < needed ) {
		// it is still not the checksum,
		incomingAsBuffer[ptrOffset] = data;
		return;
	}
	//test the checksum
	uint8_t cs;
	arke_compute_checksum(*incoming,cs);
	if (cs != data ) {
		//incorrect checksum
		//TODO: report error
		//enables new packet reception
		CHANNEL_MARK_FREE(link.received);
		return;
	}

	if (link.received >= HOST_PACKET_SIZE(*incoming) ) {
		//marks a valid packet received in the incoming ringbuffer
		RB_INCREMENT_TAIL(link.in,RB_IN_SIZE);
		//enables a new reception
		CHANNEL_MARK_FREE(link.received);
	}

}


void ProcessIncoming() {
	yaacl_txn_status_e s = yaacl_txn_status(&link.txn);
	if (s == YAACL_TXN_PENDING ) {
		return;
	}

	if ( s == YAACL_TXN_COMPLETED ) {
		RB_INCREMENT_HEAD(link.in, RB_IN_SIZE);
	}

	if ( yaacl_txn_had_error(s) ) {
		//TODO: report error ?
		RB_INCREMENT_HEAD(link.in, RB_IN_SIZE);
	}

	if ( RB_EMPTY(link.in) ) {
		// no more new message to process
		return;
	}


	HostPacket * incoming = &RB_HEAD(link.in);

	if ( (incoming->ID & CONTROLBIT_MSK) == 0 ) {
		//data to send to the bus
		if ( (incoming->ID & IDEBIT_MSK ) == 0 ) {
			yaacl_make_std_idt(link.txn.ID,incoming->ID & STD_ID_MSK,incoming->ID & RTRBIT_MSK);
		} else {
			yaacl_make_ext_idt(link.txn.ID,incoming->ID & EXT_ID_MSK,incoming->ID & RTRBIT_MSK);
		}
		link.txn.length = MIN(incoming->length,8);
		link.txn.data = incoming->data;
		yaacl_send(&link.txn);
		return;

	}

	// This is a control packet :
	//TODO :  Incoming logic process
	uint8_t packetID = incoming->ID & 0xff;
	if(packetID == HP_RESET_REQUEST) {
		software_reset();
		RB_INCREMENT_HEAD(link.in,RB_IN_SIZE);
		return;
	}

	if (packetID == HP_FW_VERSION_REQUEST) {
		if(RB_FULL(link.out,RB_OUT_SIZE)) {
			//do not treat packet and retry
			return;
		}
		HostPacket * outgoing = &RB_TAIL(link.out);
		arke_hp_make_fw_version(*outgoing,ARKE_FW_MAJOR,ARKE_FW_MINOR);
		RB_INCREMENT_TAIL(link.out,RB_OUT_SIZE);
		RB_INCREMENT_HEAD(link.in,RB_IN_SIZE);
		return;
	}

	// TODO: repport unknown packet ?

	RB_INCREMENT_HEAD(link.in,RB_IN_SIZE);
}

// Process incoming packet from host
void ProcessHostLink() {
	ProcessHostTx();
	ProcessHostRx();
	ProcessIncoming();
}


// Prepares an upstreamn
int HostSendCANPacket(const yaacl_txn_t * txn) {
	if(RB_FULL(link.out,RB_OUT_SIZE)) {
		// TODO report error
		return 1;
	}
	HostPacket * data = &RB_TAIL(link.out);
	if (txn->ID.std.rb0 ) {
		data->ID = txn->ID.ext.ID;
		data->ID |= IDEBIT_MSK;
		if (txn->ID.ext.rtr != 0 ) {
			data->ID |= RTRBIT_MSK;
		} else {
			data->ID &= ~(RTRBIT_MSK);
		}
	} else {
		data->ID = txn->ID.std.ID;
		data->ID &= ~(IDEBIT_MSK);
		if (txn->ID.std.rtr != 0 ) {
			data->ID |= RTRBIT_MSK;
		} else {
			data->ID &= ~(RTRBIT_MSK);
		}
	}

	data->length = txn->length;
	for (uint8_t i = 0; i < txn->length; ++i ) {
		data->data[i] = txn->data[i];
	}

	RB_INCREMENT_TAIL(link.out,RB_OUT_SIZE);

	return 0;
}

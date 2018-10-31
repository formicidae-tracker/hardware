#include "HostLink.h"

#include <yaacl.h>

#include "SoftReset.h"
#include "LEDs.h"

#include <HostCommunication.h>

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

// UART is slower than the bus, so we buffer more outgoing than
// ingoing.
#define RB_IN_SIZE 256
#define RB_OUT_SIZE 256
#define RB_MASK(size) (size-1)

#define MAX(a,b) ( ((a) > (b)) ? (a) : (b) )
#define MIN(a,b) ( ((a) < (b)) ? (a) : (b) )

#define CHANNEL_IS_FREE(ch) (ch == 0xff)
#define CHANNEL_MARK_FREE(ch) ch = 0xff;


typedef struct HostPacketInRingBuffer {
	uint8_t data[RB_IN_SIZE];
	uint8_t head;
	uint8_t tail;
	uint16_t size;
} HostPacketInRingBuffer;

typedef struct HostPacketOutRingBuffer {
	uint8_t data[RB_OUT_SIZE];
	uint8_t head;
	uint8_t tail;
	uint16_t size;
} HostPacketOutRingBuffer;


#define SLCAN_ACK  '\r'
#define SLCAN_NACK '\a'

#define RB_INIT(rb) do {	  \
		(rb).head = 0; \
		(rb).tail = 0; \
		(rb).size = 0; \
	}while(0)
#define RB_HEAD(rb) ((rb).data[(rb).head])
#define RB_TAIL(rb) ((rb).data[(rb).tail])
#define RB_EMPTY(rb) ((rb).size == 0)
#define RB_FULL(rb,size_) ((rb).size == size_)
#define RB_INCREMENT_TAIL(rb,size_) do {	  \
		++(rb).size; \
		(rb).tail = ((rb).tail + 1); \
	}while(0)

#define RB_INCREMENT_HEAD(rb,size_) do {	  \
		--(rb).size; \
		(rb).head = ((rb).head + 1); \
	}while(0)


typedef struct HostData {
	HostPacketInRingBuffer in;
	HostPacketOutRingBuffer out;
	yaacl_txn_t tx[3];
	yaacl_txn_t rx[3];
	uint8_t  status;
	uint8_t  errorStatus;
	uint8_t pendingRx;
	uint8_t txStatus;
} HostData;
HostData link;

void HostReportHostRxBufferOverflow();
void HostReportHostSOFMissing();
void HostReportHostRxInvalidChecksum();
void HostReportHostRxUnknownPacketID();
void HostReportHostTxBufferOverflow();


void HostSendNack() {
	if(RB_FULL(link.out,RB_OUT_SIZE) ) {
		HostReportHostTxBufferOverflow();
	}
	RB_TAIL(link.out) = SLCAN_NACK;
	RB_INCREMENT_TAIL(link.out,RB_OUT_SIZE);
}


// Init the UART link.
void InitHostLink() {
	RB_INIT(link.in);
	RB_INIT(link.out);

	for ( uint8_t i = 0; i < 3; ++i) {
		yaacl_init_txn(&link.tx[i]);
		yaacl_init_txn(&link.rx[i]);
	}

	link.status = 0;
	link.errorStatus = 0;
	link.pendingRx = 0;
	link.txStatus = 0;


	// set baudrate to 114285 within 0.79% of 115200
	// 115200 would often be the maximal value any UART would reach in
	// modern PC
	// BAUD = FIO / ( LBT * (LDIV + 1) )
	// LBT = 35 LDIV = 3
	LINBTR = _BV(LDISR) | 34; // LBT
	LINBRR = 3; //

	// enable UART Rx and Tx mode
	LINCR = _BV(LENA) | _BV(LCMD2) |  _BV(LCMD1) |  _BV(LCMD0) ;
}

void ProcessHostTx() {
	if ( (link.txStatus != 0) && (LINSIR & _BV(LTXOK) ) == 0x00 ) {
		// we are TX-ing out
		return;
	}

	// nothing to transmit
	if ( RB_EMPTY(link.out) ) {
		link.txStatus = 0;
		return;
	}

	LINDAT = RB_HEAD(link.out);
	RB_INCREMENT_HEAD(link.out,RB_OUT_SIZE);
	link.txStatus = 1;

}
uint8_t count = 0;
char * countStr = "000";
void ProcessHostRx() {
	if ( (LINSIR & _BV(LRXOK) ) == 0x00 ) {
		return;
	}


	if ( link.out.size >= 250) {
		//HostReportHostRxBufferOverflow();
		LEDErrorBlink(2);
		return;
	}
	++count;
	uint8_t data = LINDAT;
	//RB_TAIL(link.out) = ':';
	//RB_INCREMENT_TAIL(link.out,RB_OUT_SIZE);
	RB_TAIL(link.out) = LINDAT;
	RB_INCREMENT_TAIL(link.out,RB_OUT_SIZE);
	if ( data == ';' ) {
		sprintf(countStr,"%03d",count);
		RB_TAIL(link.out) = countStr[0];
		RB_INCREMENT_TAIL(link.out,RB_OUT_SIZE);
		RB_TAIL(link.out) = countStr[1];
		RB_INCREMENT_TAIL(link.out,RB_OUT_SIZE);
		RB_TAIL(link.out) = countStr[2];
		RB_INCREMENT_TAIL(link.out,RB_OUT_SIZE);
	}
	//RB_TAIL(link.out) = '\n';
	//RB_INCREMENT_TAIL(link.out,RB_OUT_SIZE);
	LEDDataToggle();

	/* uint8_t received = LINDAT; */
	/* RB_TAIL(link.in) = received; */

	/* RB_INCREMENT_TAIL(link.in,RB_IN_SIZE); */

	/* if ( received == SLCAN_ACK ) { */
	/* 	++link.pendingRx; */
	/* 	// */
	/* } */
}


void ProcessIncoming() {
	if ( link.pendingRx == 0 ) {
		return;
	}


	//check internal soundness of buffer
	uint8_t size;
	bool found = false;
	for (size = 0;
	     size < link.in.size;
	     ++size) {
		if (link.in.data[(link.in.head + size) & RB_MASK(RB_IN_SIZE) ] == SLCAN_ACK ) {
			found = true;
			break;
		}
	}

	if (found == false ) {
		//Bad internal issue
		//TODO: report issue
		RB_INIT(link.in);
		return;
	}

	// treat command
	switch ( RB_HEAD(link.in) ) {
	case 'O':
	case 'C':
	case 'F':
	case 'V':
	case 'S':
	case 's':
	case 't':
	case 'T':
	case 'r':
	case 'R':
		//none implemented yet
		HostSendNack();
		break;
	default:
		HostSendNack();
	}

	link.in.head = (link.in.head + size + 1) & RB_MASK(RB_IN_SIZE);
	link.in.size -= size + 1;
	--link.pendingRx;


}

// Process incoming packet from host
void ProcessHostLink() {
	ProcessHostTx();
	ProcessHostRx();
	ProcessIncoming();
	if( (LINSIR & _BV(LERR)) != 0 ) {
		uint8_t foo = LINDAT;
		if (foo == 0 ) {
			LEDDataOn();
		}
		LEDErrorToggle();
	}
}


// Prepares an upstreamn
int HostSendCANPacket(const yaacl_txn_t * txn) {
	return 0;
}


//ERROR LED priority, Higher the winner
typedef enum {
	LED_ERR_HOST_RX_PRIORITY = 1,
	LED_ERR_CAN_TXN_ERR_PRIORITY = 2,
	LED_ERR_BUFFER_OVERFLOW = 3,
} LEDErrorBlinkPriority;


void HostReportCANRxError() {
	LEDErrorBlink(LED_ERR_CAN_TXN_ERR_PRIORITY);
}
void HostReportCANTxError() {
	LEDErrorBlink(LED_ERR_CAN_TXN_ERR_PRIORITY);
}

void HostReportHostRxBufferOverflow() {
	LEDErrorBlink(LED_ERR_BUFFER_OVERFLOW);
}
void HostReportHostSOFMissing() {

}
void HostReportHostRxInvalidChecksum() {
}
void HostReportHostRxUnknownPacketID() {
	LEDErrorBlink(LED_ERR_HOST_RX_PRIORITY);
}
void HostReportHostTxBufferOverflow() {
	LEDErrorBlink(LED_ERR_BUFFER_OVERFLOW);
}

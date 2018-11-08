#include "HostLink.h"

#include <yaacl.h>

#include "SoftReset.h"
#include "LEDs.h"

#include <HostCommunication.h>

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include <stdio.h>

// UART is slower than the bus, so we buffer more outgoing than
// ingoing.
#define RB_IN_SIZE 256
#define RB_OUT_SIZE 256
#define RB_MASK(size) (size-1)

#define NB_TXN (YAACL_NB_MOB / 2)


#define MAX(a,b) ( ((a) > (b)) ? (a) : (b) )
#define MIN(a,b) ( ((a) < (b)) ? (a) : (b) )

#define CHANNEL_IS_FREE(ch) (ch == 0xff)
#define CHANNEL_MARK_FREE(ch) ch = 0xff;


uint8_t hex_to_bin(char c) {
	if ( (c >= '0') && (c <= '9')) {
		return c - '0';
	}
	if ( (c >= 'A') && (c <= 'F')) {
		return c - 'A' + 10;
	}
	return 0xff;
}

char hexchar[16] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};

char bin_to_hex(uint8_t i) {
	return hexchar[i & 0x0f];
}


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

#define RB_INIT(rb) do {	  \
		(rb).head = 0; \
		(rb).tail = 0; \
		(rb).size = 0; \
	}while(0)
#define RB_HEAD(rb) ((rb).data[(rb).head])
#define RB_FROM_HEAD(rb,idx,size_) ((rb).data[((rb).head + idx) & RB_MASK(size_)])
#define RB_TAIL(rb) ((rb).data[(rb).tail])
#define RB_EMPTY(rb) ((rb).size == 0)
#define RB_FULL(rb,size_) ((rb).size == size_)
#define RB_INCREMENT_TAIL(rb,size_) do {	  \
		++(rb).size; \
		(rb).tail = ((rb).tail + 1) & RB_MASK(size_); \
	}while(0)

#define RB_INCREMENT_HEAD(rb,size_) do {	  \
		--(rb).size; \
		(rb).head = ((rb).head + 1) & RB_MASK(size_); \
	}while(0)


enum SlcanStatus {
	SLCAN_CLOSED = 0x00,
	SLCAN_RX = 0x01,
	SLCAN_TX = 0x02
};

enum SlcanBaudrate {
	S0 = '0',
	S1 = '1',
	S2 = '2',
	S3 = '3',
	S4 = '4',
	S5 = '5',
	S6 = '6',
	S7 = '7',
	S8 = '8',
	SLCAN_UNKNOWN_BAUDRATE = 0,
	SLCAN_NB_BAUDRATE = 9
};

#define UNSUPPORTED YAACL_NB_SUPPORTED_BAUDRATE

yaacl_baudrate_e supportedBaudrate[9] = {
	UNSUPPORTED,
	UNSUPPORTED,
	UNSUPPORTED,
	YAACL_BR_100,
	YAACL_BR_125,
	YAACL_BR_250,
	YAACL_BR_500,
	UNSUPPORTED,
	YAACL_BR_1000
};


typedef struct HostData {
	volatile HostPacketInRingBuffer in;
	HostPacketOutRingBuffer out;
	yaacl_txn_t tx[NB_TXN];
	yaacl_txn_t rx[NB_TXN];
	uint8_t     txData[NB_TXN*8];
	uint8_t     rxData[NB_TXN*8];
	enum SlcanBaudrate canBaudrate;
	enum SlcanStatus status;
	uint8_t  errorStatus;
	volatile uint8_t pendingRx;
	uint8_t txStatus;
} HostData;
HostData link;



#define SLCAN_ACK  '\r'
#define SLCAN_NACK '\a'

#define HostSendSingleCharUnsafe(c) do {	  \
		RB_TAIL(link.out) = c; \
		RB_INCREMENT_TAIL(link.out,RB_OUT_SIZE); \
	}while(0)

// Init the UART link.
void InitHostLink() {
	RB_INIT(link.in);
	RB_INIT(link.out);

	link.status = SLCAN_CLOSED;
	link.errorStatus = 0;
	link.pendingRx = 0;
	link.txStatus = 0;
	link.canBaudrate = SLCAN_UNKNOWN_BAUDRATE;
	// set baudrate to 114285 within 0.79% of 115200
	// 115200 would often be the maximal value any UART would reach in
	// modern PC
	// BAUD = FIO / ( LBT * (LDIV + 1) )
	// LBT = 35 LDIV = 3
	LINBTR = _BV(LDISR) | 34; // LBT
	LINBRR = 3; //

	// enable UART Rx and Tx mode
	LINCR = _BV(LENA) | _BV(LCMD2) |  _BV(LCMD1) |  _BV(LCMD0) ;

	LINENIR = _BV(LENERR) | _BV(LENRXOK);
	sei();
}

volatile uint8_t discarder;

typedef enum {
	ARKE_RX_ERROR = (1<<0),
	ARKE_RX_OVERFLOW_ERROR = (1<<1),
	ARKE_TX_OVERFLOW_ERROR = (1<<2),
	ARKE_INTERNAL_ERROR = (1<<3),
	ARKE_CAN_TX_ACK_ERROR = (1<<4),
	ARKE_CAN_TX_ERROR = (1<<5),
	ARKE_CAN_RX_CRC_ERROR = (1<<6),
	ARKE_CAN_RX_ERROR = (1<<7)
} ArkeStatusFlags_e;


void HostReportUARTRxError();
void HostReportRxOverflow();
void HostReportTxBufferOverflow();
void HostReportInternalError();
void HostReportCANTxError(yaacl_txn_status_e s);
void HostReportCANRxError(yaacl_txn_status_e s);


ISR(LIN_ERR_vect) {
	HostReportUARTRxError();
	discarder = LINDAT;
}

ISR(LIN_TC_vect) {
	uint8_t data = LINDAT;
	if ( RB_FULL(link.in,RB_IN_SIZE) ) {
		HostReportRxOverflow();
		return;
	}

	RB_TAIL(link.in) = data;
	RB_INCREMENT_TAIL(link.in,RB_IN_SIZE);
	if (data == SLCAN_ACK) {
		++link.pendingRx;
	}

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


void HostInitYaacl() {
	yaacl_config_t config;
	config.baudrate = supportedBaudrate[link.canBaudrate-S0];
	yaacl_init(&config);

	for ( uint8_t i = 0; i < NB_TXN; ++i) {
		yaacl_init_txn(&link.tx[i]);
		yaacl_init_txn(&link.rx[i]);
		link.rx[i].length = 8;
		link.rx[i].data = &(link.rxData[8*i]);
		link.tx[i].data = &(link.txData[8*i]);
		yaacl_make_ext_idt(link.rx[i].ID,0,0);
		yaacl_make_ext_mask(link.rx[i].mask,0,0,0);
		yaacl_listen(&link.rx[i]);
	}

}


bool HostTestAndRespondTx(yaacl_txn_status_e s, bool idebit) {
	if ( s == YAACL_TXN_UNSUBMITTED ) {
		return true;
	}
	if (s == YAACL_TXN_PENDING ) {
		// wait for completion
		return false;
	}

	if ( link.out.size > RB_OUT_SIZE - 2 ) {
		HostReportTxBufferOverflow();
		return true;
	}

	if ( s == YAACL_TXN_COMPLETED ) {
		HostSendSingleCharUnsafe( idebit ? 'Z' : 'z');
		HostSendSingleCharUnsafe( SLCAN_ACK );
	} else {
		HostReportCANTxError(s);
		HostSendSingleCharUnsafe( SLCAN_NACK );
	}
	return true;
}


uint8_t HostNextFreeTx() {
	for (uint8_t i = 0; i < NB_TXN ; ++i ) {
		yaacl_txn_status_e s = yaacl_txn_status(&link.tx[i]);
		if ( HostTestAndRespondTx(s,yaacl_idt_test_idebit(link.tx[i].ID)) ) {
			return i;
		}
	}
	//none free
	return 0xff;
}


bool HostUnsupportedCommand() {
	HostSendSingleCharUnsafe(SLCAN_NACK);
	return true;
}

bool HostOpenCommand(uint8_t commandSize) {
	if ( commandSize != 1
	     || link.status != SLCAN_CLOSED
	     || link.canBaudrate == SLCAN_UNKNOWN_BAUDRATE ) {
		HostSendSingleCharUnsafe(SLCAN_NACK);
		return true;
	}
	link.status = SLCAN_TX | SLCAN_RX;

	HostInitYaacl();
	HostSendSingleCharUnsafe(SLCAN_ACK);

	return true;
}


bool HostListenCommand(uint8_t commandSize){
	if ( commandSize != 1
	     || link.status != SLCAN_CLOSED
	     || link.canBaudrate == SLCAN_UNKNOWN_BAUDRATE ) {
		HostSendSingleCharUnsafe(SLCAN_NACK);
		return true;
	}

	link.status = SLCAN_RX;
	HostInitYaacl();
	HostSendSingleCharUnsafe(SLCAN_ACK);

	return true;
}

bool HostCloseCommand(uint8_t commandSize) {
	//we can always close something closed
	if ( commandSize != 1 ) {
		HostSendSingleCharUnsafe(SLCAN_NACK);
		return true;
	}

	yaacl_deinit();
	link.status = SLCAN_CLOSED;
	link.errorStatus = 0;
	LEDErrorOff();
	HostSendSingleCharUnsafe(SLCAN_ACK);
	return true;
}

bool HostStatusCommand(uint8_t commandSize) {
	if( commandSize != 1 ||
	    link.status == SLCAN_CLOSED) {
		HostSendSingleCharUnsafe(SLCAN_NACK);
		return true;
		}
	if ( link.out.size > RB_OUT_SIZE - 4) {
		HostReportTxBufferOverflow();
		// we will treat the packet later when the out buffer will
		// be free.
		return false;
	}

	HostSendSingleCharUnsafe('F');
	HostSendSingleCharUnsafe(bin_to_hex(link.errorStatus>>4));
	HostSendSingleCharUnsafe(bin_to_hex(link.errorStatus>>0));
	link.errorStatus = 0;
	LEDErrorOff();
	HostSendSingleCharUnsafe(SLCAN_ACK);
	return true;
}

bool HostVersionCommand(uint8_t commandSize) {
	if (commandSize > 1) {
		HostSendSingleCharUnsafe(SLCAN_NACK);
		return true;
	}
	if (link.out.size > RB_OUT_SIZE - 6) {
		HostReportTxBufferOverflow();
		// we will treat the packet later when the out buffer will
		// be free.
		return false;
	}
	HostSendSingleCharUnsafe('V');
	HostSendSingleCharUnsafe('4');
	HostSendSingleCharUnsafe('3');
	HostSendSingleCharUnsafe('2');
	HostSendSingleCharUnsafe('1');
	HostSendSingleCharUnsafe(SLCAN_ACK);

	return true;
}


bool HostBaudrateSelectCommand(uint8_t commandSize) {
	if (commandSize != 2
	    || link.status != SLCAN_CLOSED ) {
		HostSendSingleCharUnsafe(SLCAN_NACK);
		return true;
	}
	uint8_t value = RB_FROM_HEAD(link.in,1,RB_IN_SIZE) - S0;
	if (value >= SLCAN_NB_BAUDRATE || supportedBaudrate[value] == UNSUPPORTED ) {
		HostSendSingleCharUnsafe(SLCAN_NACK);
		return true;
	}
	link.canBaudrate = value + S0;
	HostSendSingleCharUnsafe(SLCAN_ACK);
	return true;
}


bool HostCANTxCommand(uint8_t commandSize, bool idebit, bool rtrbit) {
	if ( (link.status && SLCAN_TX) == 0x00 ) {
		HostSendSingleCharUnsafe(SLCAN_NACK);
		return true;
	}

	uint8_t next = HostNextFreeTx();
	if ( next == 0xff ) {
		// we come up when we have one free
		return false;
	}

	uint8_t expectedSize = 4 ;
	if ( idebit ) {
		expectedSize = 9 ;
	}
	if (commandSize <= expectedSize ) {
		HostSendSingleCharUnsafe(SLCAN_NACK);
		return true;
	}

	uint8_t dlc = RB_FROM_HEAD(link.in,expectedSize,RB_IN_SIZE) - '0';
	if ( dlc < 0 || dlc > 8 || commandSize != expectedSize + 2 * dlc + 1 ) {
		HostSendSingleCharUnsafe(SLCAN_NACK);
		return true;
	}

	uint32_t ID = 0;

	for (uint8_t i = 1; i < expectedSize; ++i) {
		uint8_t toAdd = hex_to_bin(RB_FROM_HEAD(link.in,i,RB_IN_SIZE));
		if (toAdd == 0xff) {
			HostSendSingleCharUnsafe(SLCAN_NACK);
			return true;
		}
		if ( i == 1) {
			if ( (idebit  && (toAdd > 0x01) ) || ( (!idebit) && (toAdd > 0x07) ) ) {
				//wrong ID
				HostSendSingleCharUnsafe(SLCAN_NACK);
				return true;
			}
		} else {
			ID = ID << 4;
		}
		ID |= toAdd;
	}

	if (idebit) {
		yaacl_make_ext_idt(link.tx[next].ID,ID,rtrbit);
	} else {
		yaacl_make_std_idt(link.tx[next].ID,ID,rtrbit);
	}
	link.tx[next].length = dlc;

	for (uint8_t i = 0; i < dlc; ++i) {
		uint8_t MSB = hex_to_bin(RB_FROM_HEAD(link.in,expectedSize + 1 + 2*i,RB_IN_SIZE));
		if ( MSB == 0xff ) {
			HostSendSingleCharUnsafe(SLCAN_NACK);
			return true;
		}
		uint8_t LSB = hex_to_bin(RB_FROM_HEAD(link.in,expectedSize + 2 + 2*i,RB_IN_SIZE));
		if (LSB == 0xff ) {
			HostSendSingleCharUnsafe(SLCAN_NACK);
			return true;
		}
		link.tx[next].data[i] = (MSB << 4) | LSB;
	}

	yaacl_send(&link.tx[next]);
	// DO NOT SEND ACKNOWLEDGEMNT YET
	// Wait for CAN txn result

	return true;
}

bool HostFullResetCommand(uint8_t commandSize) {
		if ( commandSize != 9  ||
		    link.in.data[(link.in.head + 1) & RB_MASK(RB_IN_SIZE)] != 'R' ||
		    link.in.data[(link.in.head + 2) & RB_MASK(RB_IN_SIZE)] != 'E' ||
		    link.in.data[(link.in.head + 3) & RB_MASK(RB_IN_SIZE)] != 'S' ||
		    link.in.data[(link.in.head + 4) & RB_MASK(RB_IN_SIZE)] != 'E' ||
		    link.in.data[(link.in.head + 5) & RB_MASK(RB_IN_SIZE)] != 'T' ||
		    link.in.data[(link.in.head + 6) & RB_MASK(RB_IN_SIZE)] != 'N' ||
		    link.in.data[(link.in.head + 7) & RB_MASK(RB_IN_SIZE)] != 'O' ||
		    link.in.data[(link.in.head + 8) & RB_MASK(RB_IN_SIZE)] != 'W' ) {
			HostSendSingleCharUnsafe(SLCAN_NACK);
			return true;
		}
		software_reset();
		return true;
}

void ProcessIncoming() {
	cli();
	uint8_t inSize = link.in.size;
	if ( link.pendingRx == 0 ) {
		sei();
		return;
	}

	//check internal soundness of buffer
	uint8_t commandSize;
	bool found = false;
	for (commandSize = 0;
	     commandSize < inSize;
	     ++commandSize) {
		if (RB_FROM_HEAD(link.in,commandSize,RB_IN_SIZE) == SLCAN_ACK ) {
			found = true;
			break;
		}
	}

	if (found == false ) {
		//Bad internal issue
		HostReportInternalError();
		cli();
		RB_INIT(link.in);
		link.pendingRx = 0;
		sei();
		return;
	}

	if ( RB_FULL(link.out,RB_OUT_SIZE) ) {
		HostReportTxBufferOverflow();
		return;
	}

	bool messageIsProcessed;

	// treat command


	switch ( RB_HEAD(link.in) ) {
	case 'O':
		messageIsProcessed = HostOpenCommand(commandSize);
		break;
	case 'L':
		messageIsProcessed = HostListenCommand(commandSize);
		break;
	case 'C':
		messageIsProcessed = HostCloseCommand(commandSize);
		break;
	case 'F':
		messageIsProcessed = HostStatusCommand(commandSize);
		break;
	case 'V':
		messageIsProcessed = HostVersionCommand(commandSize);
		break;
	case 'S':
		messageIsProcessed = HostBaudrateSelectCommand(commandSize);
		break;
	case 's':
		messageIsProcessed = HostUnsupportedCommand();
		break;
	case 'T':
	case 'R':
	case 't':
	case 'r':
		messageIsProcessed = HostCANTxCommand(commandSize,
		                                      RB_HEAD(link.in) == 'T' || RB_HEAD(link.in) == 'R',
		                                      RB_HEAD(link.in) == 'r' || RB_HEAD(link.in) == 'R');
		break;
	case 'W':
		messageIsProcessed = HostFullResetCommand(commandSize);
		break;
	default:
		messageIsProcessed = HostUnsupportedCommand();
	}

	if ( ! messageIsProcessed ) {
		// the command lack room to do its job, retry !!!
		return;
	}

	sei();
	link.in.head = (link.in.head + commandSize + 1) & RB_MASK(RB_IN_SIZE);
	link.in.size -= commandSize + 1;
	--link.pendingRx;
	cli();

}

void ProcessCan() {
	for (uint8_t i = 0; i < NB_TXN; ++i ) {
		yaacl_txn_status_e s = yaacl_txn_status(&link.tx[i]);
		HostTestAndRespondTx(s, yaacl_idt_test_idebit(link.tx[i].ID));
	}


	for (uint8_t i = 0; i < NB_TXN; ++i ) {
		yaacl_txn_status_e s = yaacl_txn_status(&link.rx[i]);

		if ( s == YAACL_TXN_PENDING ) {
			continue;
		}

		if ( s == YAACL_TXN_COMPLETED || s == YAACL_TXN_UNSUBMITTED) {
			uint8_t size = 6;

			char cmd = 't';
			if (yaacl_idt_test_rtrbit(link.rx[i].ID) ) {
				cmd = 'r';
			}
			if ( yaacl_idt_test_idebit(link.rx[i].ID) ) {
				size += 5;
				if (cmd == 't' ) {
					cmd = 'T';
				} else {
					cmd = 'R';
				}
			}

			if (link.out.size >= RB_OUT_SIZE - size - 2 * link.rx[i].length ) {
				HostReportTxBufferOverflow();
				continue;
			}


			HostSendSingleCharUnsafe(cmd);
			if ( yaacl_idt_test_idebit(link.rx[i].ID) ) {
				HostSendSingleCharUnsafe( bin_to_hex((link.rx[i].ID >> 28) & 0x01 ));
				HostSendSingleCharUnsafe( bin_to_hex(link.rx[i].ID >> 24) );
				HostSendSingleCharUnsafe( bin_to_hex(link.rx[i].ID >> 20) );
				HostSendSingleCharUnsafe( bin_to_hex(link.rx[i].ID >> 16) );
				HostSendSingleCharUnsafe( bin_to_hex(link.rx[i].ID >> 12) );
				HostSendSingleCharUnsafe( bin_to_hex(link.rx[i].ID >> 8) );
				HostSendSingleCharUnsafe( bin_to_hex(link.rx[i].ID >> 4) );
				HostSendSingleCharUnsafe( bin_to_hex(link.rx[i].ID) );
			} else {
				HostSendSingleCharUnsafe( bin_to_hex((link.rx[i].ID >> 8) & 0x07) );
				HostSendSingleCharUnsafe( bin_to_hex(link.rx[i].ID >> 4) );
				HostSendSingleCharUnsafe( bin_to_hex(link.rx[i].ID) );
			}
			HostSendSingleCharUnsafe( link.rx[i].length );
			for (uint8_t i = 0 ; i < link.rx[i].length; ++i ) {
				HostSendSingleCharUnsafe( bin_to_hex(link.rx[i].ID >> 4) );
				HostSendSingleCharUnsafe( bin_to_hex(link.rx[i].ID) );
			}
			HostSendSingleCharUnsafe(SLCAN_ACK);

		} else {
			HostReportCANRxError(s);
		}

		//re-init the transaction as soon as possible
		yaacl_make_ext_idt(link.rx[i].ID,0,0);
		link.rx[i].length = 8;
		yaacl_listen(&link.rx[i]);

	}


}

// Process incoming packet from host
void ProcessHostLink() {
	ProcessHostTx();
	ProcessIncoming();
	ProcessCan();

	if ( RB_FULL(link.in,RB_IN_SIZE) || RB_FULL(link.out,RB_OUT_SIZE) ) {
		LEDReadyOff();
	} else {
		LEDReadyOn();
	}

	if ( RB_EMPTY(link.in) && RB_EMPTY(link.out) ) {
		LEDDataOff();
	} else {
		LEDDataOn();
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

#define set_error_led_blink() do{	  \
		uint8_t count = 0; \
		for (uint8_t i = 0; i < 8; ++i) { \
			if ( (link.errorStatus & _BV(i)) != 0 ) { \
				++count; \
			} \
		} \
		LEDErrorBlink(count); \
	}while(0)


void HostReportUARTRxError() {
	link.errorStatus |= ARKE_RX_ERROR;
	set_error_led_blink();
}

void HostReportRxOverflow() {
	link.errorStatus |= ARKE_RX_OVERFLOW_ERROR;
	set_error_led_blink();
}

void HostReportTxBufferOverflow() {
	link.errorStatus |= ARKE_TX_OVERFLOW_ERROR;
	set_error_led_blink();
}

void HostReportInternalError() {
	link.errorStatus |= ARKE_INTERNAL_ERROR;
	set_error_led_blink();
}

void HostReportCANTxError(yaacl_txn_status_e s) {
	link.errorStatus |= ARKE_CAN_TX_ERROR;
	if ( (s & YAACL_TXN_ACK_ERROR) != 0 ) {
		link.errorStatus |= ARKE_CAN_TX_ACK_ERROR;
	}
	set_error_led_blink();
}

void HostReportCANRxError(yaacl_txn_status_e s) {
	link.errorStatus |= ARKE_CAN_RX_ERROR;
	if ( (s & YAACL_TXN_CRC_ERROR) != 0 ) {
		link.errorStatus |= ARKE_CAN_RX_CRC_ERROR;
	}
	set_error_led_blink();
}

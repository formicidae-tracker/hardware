#include "yaail.h"

#include <util/twi.h>
#include <avr/interrupt.h>

#ifndef YAAIL_FIFO_SIZE
//USING default FIFO size

#define YAAIL_FIFO_SIZE 64
#endif

#if YAAIL_FIFO_SIZE > 128
#error "Maximum YAAIL_FIFO_SIZE is 128"
#endif

#define is_power_of_two(x) (x == 2 || x == 4 || x == 8 || x == 16 || x == 32 || x == 64 || x == 128 )

#if is_power_of_two(YAAIL_FIFO_SIZE)
#define YAAIL_FIFO_MASK (YAAIL_FIFO_SIZE-1)
#else
#error "YAAIL_FIFO_SIZE_ must be a power of two"
#endif

#if F_CPU==16000000L
uint8_t brr[4] = {10,18,36,72};
#else
#error "Unsupported F_CPU"
#endif


enum yaail_status_e {
	YAAIL_NONE = 0,
	YAAIL_WAITING_START = 1,
	YAAIL_WAITING_SLA = 2,
	YAAIL_WAITING_DATA = 3,
	YAAIL_WAITING_REARM = 4,
	YAAIL_WAITING_RESTART = 5,
	YAAIL_WAITING_STOP = 6
};


struct yaail_private_txn_t{
	uint8_t address;
	uint8_t length,lengthRead;
	uint8_t * data;
	volatile yaail_txn_t * txn;
};

struct yaail_txn_fifo_t {
	struct yaail_private_txn_t data[YAAIL_FIFO_SIZE];
	volatile uint8_t size,head,tail;
};



struct yaail_t {
	enum yaail_status_e status;
	uint8_t done;

	struct yaail_txn_fifo_t fifo;
};

#define FIFO_INIT(f) do {	  \
		(f).size = 0; \
		(f).head = 0; \
		(f).tail = 0; \
	}while(0)
#define FIFO_FULL(f) ((f).size >= YAAIL_FIFO_SIZE)
#define FIFO_EMPTY(f) ((f).size == 0)
#define FIFO_HEAD(f) ((f).data[(f).head])
#define FIFO_TAIL(f) ((f).data[(f).tail])
#define FIFO_INCR_PTR(p) (p) = ((p) + 1) & YAAIL_FIFO_MASK
#define FIFO_INCREMENT_HEAD(f) do{	  \
		FIFO_INCR_PTR((f).head); \
		(f).size -= 1; \
	}while(0)
#define FIFO_INCREMENT_TAIL(f) do{	  \
		FIFO_INCR_PTR((f).tail); \
		(f).size += 1; \
	}while(0)


struct yaail_t il;


yaail_error_e yaail_init(yaail_baudrate_e br) {
	il.status = YAAIL_NONE;
	if (br >= YAAIL_NB_SUPPORTED_BAUDRATE) {
		return YAAIL_UNSUPPORTED_BAUDRATE_ERROR;
	}
	FIFO_INIT(il.fifo);
	TWSR = 1;
	TWBR = brr[br];
	sei();
	return YAAIL_NO_ERROR;
}

void yaail_start_pending_txn() {
	uint8_t sreg = SREG;
	cli();
	if (il.status == YAAIL_NONE && il.fifo.size > 0 ) {
		il.status = YAAIL_WAITING_START;
		*(FIFO_HEAD(il.fifo).txn) = YAAIL_PENDING;
		TWCR = _BV(TWIE) | _BV(TWINT) | _BV(TWSTA) | _BV(TWEN);
	}
	SREG = sreg;
}

#define yaail_validate_address(address) do { \
		uint8_t sreg = SREG; \
		cli(); \
		if ( FIFO_FULL(il.fifo) ) { \
			SREG = sreg; \
			return YAAIL_BUFFER_OVERFLOW_ERROR; \
		} \
		/*ok to re-enable, interrupt will only decrease the fifo*/ \
		SREG = sreg; \
		if ( (address & 0x80 ) != 0 ) { \
			return YAAIL_INVALID_ADDRESS_ERROR; \
		} \
	}while(0)


#define yaail_validate_op(address,length) do {	  \
		yaail_validate_address(address); \
		if ( length == 0 ) { \
			return YAAIL_INVALID_LENGTH_ERROR; \
		} \
	}while(0)

yaail_error_e yaail_read(yaail_txn_t * txn,
                         uint8_t address,
                         uint8_t * data, uint8_t length) {
	yaail_validate_op(address,length);
	struct yaail_private_txn_t * txnp = &FIFO_TAIL(il.fifo);
	txnp->txn = txn;
	txnp->address = (address << 1) | 0x01;
	txnp->data = data;
	txnp->length = length;
	txnp->lengthRead = length;
	*(txnp->txn) = YAAIL_SCHEDULED;
	uint8_t sreg = SREG;
	cli();
	FIFO_INCREMENT_TAIL(il.fifo);
	SREG = sreg;
	return YAAIL_NO_ERROR;
}

yaail_error_e yaail_write(yaail_txn_t * txn,
                          uint8_t address,
                          const uint8_t * data,
                          uint8_t length) {
	//we permits write of zero
	yaail_validate_address(address);
	struct yaail_private_txn_t * txnp = &FIFO_TAIL(il.fifo);
	txnp->txn = txn;
	txnp->address = address << 1;
	txnp->data = (uint8_t*)data;
	txnp->length = length;
	txnp->lengthRead = 0;
	*(txnp->txn) = YAAIL_SCHEDULED;
	uint8_t sreg = SREG;
	cli();
	FIFO_INCREMENT_TAIL(il.fifo);
	SREG = sreg;
	return YAAIL_NO_ERROR;
}

yaail_error_e yaail_write_and_read(yaail_txn_t * txn,
                                   uint8_t address,
                                   uint8_t * data,
                                   uint8_t lengthWrite,
                                   uint8_t lengthRead) {
	//we permit writes of 0;
	yaail_validate_op(address,lengthRead);
	if (lengthRead == 0) {
		return YAAIL_INVALID_LENGTH_ERROR;
	}
	struct yaail_private_txn_t * txnp = &FIFO_TAIL(il.fifo);
	txnp->txn = txn;
	txnp->address = address << 1;
	txnp->data = data;
	txnp->length = lengthWrite;
	txnp->lengthRead = lengthRead;
	*(txnp->txn) = YAAIL_SCHEDULED;
	uint8_t sreg = SREG;
	cli();
	FIFO_INCREMENT_TAIL(il.fifo);
	SREG = sreg;
	return YAAIL_NO_ERROR;
}

#define go_to_stop_twi(status_) do {\
		TWCR = _BV(TWIE) | _BV(TWINT) | _BV(TWSTO) | _BV(TWEN); \
		*(txn->txn) = status_; \
		FIFO_INCREMENT_HEAD(il.fifo); \
		il.status = YAAIL_NONE; \
	}while(0)

/* if ( FIFO_EMPTY(il.fifo) ) { \ */
/* 	il.status = YAAIL_NONE; \ */
/* 	return; \ */
/* } \ */
/* 		*(FIFO_HEAD(il.fifo).txn) = YAAIL_PENDING; \ */
/* 		il.status = YAAIL_WAITING_START; \ */
/* 		TWCR = _BV(TWIE) | _BV(TWINT) | _BV(TWSTA) | _BV(TWEN); \ */


ISR(TWI_vect) {
	if (FIFO_EMPTY(il.fifo)) {
		il.status = YAAIL_NONE;
		TWCR = _BV(TWINT); //clear flag
		return;
	}

	struct yaail_private_txn_t * txn = &FIFO_HEAD(il.fifo);
	if ( il.status == YAAIL_WAITING_START ) {
		if ( (TWSR & 0xf8) != TW_START && (TWSR &0xf8) != TW_REP_START ) {
			*(txn->txn) = YAAIL_START_ERROR;
			FIFO_INCREMENT_HEAD(il.fifo);
			if ( FIFO_EMPTY(il.fifo) ) {
				il.status = YAAIL_NONE;
				TWCR = _BV(TWINT);
				return;
			} else {
				TWCR = _BV(TWIE) | _BV(TWINT) | _BV(TWSTA) | _BV(TWEN);
				*(FIFO_HEAD(il.fifo).txn) = YAAIL_PENDING;
				return;
			}
		}
		il.status = YAAIL_WAITING_SLA;
		TWDR = txn->address;
		TWCR = _BV(TWIE) | _BV(TWINT) | _BV(TWEN) ;
		return;
	}

	if ( (txn->address & 0x01) == 0x00) {
		//Write case
		switch(il.status) {
		case YAAIL_WAITING_SLA:
			if ( (TWSR & 0xf8) != TW_MT_SLA_ACK ) {
				go_to_stop_twi(YAAIL_SLA_ACK_ERROR);
				return;
			}
			il.status = YAAIL_WAITING_DATA;
			il.done = 0x0;
			TWDR = txn->data[il.done];
			TWCR = _BV(TWIE) | _BV(TWINT) | _BV(TWEN);
			break;
		case YAAIL_WAITING_DATA:
			if ( (TWSR & 0xf8) != TW_MT_DATA_ACK) {
				go_to_stop_twi(YAAIL_DATA_ACK_ERROR);
				return;
			}
			if ( ++il.done >= txn->length ) {
				if ( txn->lengthRead != 0 ) {
					TWCR = _BV(TWIE) | _BV(TWINT) | _BV(TWEN) | _BV(TWSTA) ;
					txn->address = txn->address | 0x01;
					il.status = YAAIL_WAITING_START;
					txn->length = txn->lengthRead;
					return;
				} else {
					go_to_stop_twi(YAAIL_DONE);
					return;
				}
			}
			TWDR = txn->data[il.done];
			TWCR = _BV(TWIE) | _BV(TWINT) | _BV(TWEN);
			break;
		default:
			break;
		}
	} else { // Read case
		switch(il.status) {
		case YAAIL_WAITING_SLA:
			if ( (TWSR & 0xf8) != TW_MR_SLA_ACK ) {
				go_to_stop_twi(YAAIL_SLA_ACK_ERROR);
				return;
			}
			il.done = 0x0;
			il.status = YAAIL_WAITING_DATA;
			if (txn->length > 1) {
				TWCR = _BV(TWIE) | _BV(TWINT) | _BV(TWEN) | _BV(TWEA);
			} else {
				TWCR = _BV(TWIE) | _BV(TWINT) | _BV(TWEN);
			}
			break;
		case YAAIL_WAITING_DATA:
			if ( ( (il.done + 1 >= txn->length)
			       && (TWSR & 0xf8) != TW_MR_DATA_NACK) ||
			     ( (il.done + 1 <  txn->length)
			       && (TWSR & 0xf8) != TW_MR_DATA_ACK ) ) {
				go_to_stop_twi(YAAIL_DATA_ACK_ERROR);
				return;
			}
			txn->data[il.done] = TWDR;
			if (++il.done >= txn->length) {
				go_to_stop_twi(YAAIL_DONE);
				return;
			}
			if ( il.done + 1 < txn->length ) {
				TWCR = _BV(TWIE) | _BV(TWINT) | _BV(TWEN) | _BV(TWEA);
			} else {
				TWCR = _BV(TWIE) | _BV(TWINT) | _BV(TWEN);
			}
			break;
		default:
			break;
		}
	}
}



yaail_txn_status_e yaail_txn_status(const yaail_txn_t * txn) {
	uint8_t sreg = SREG;
	cli();
	yaail_txn_status_e ret = *txn;
	SREG = sreg;
	return ret;
}

yaail_txn_status_e yaail_spin_until_done(const yaail_txn_t * txn) {
	yaail_txn_status_e s;
	do {
		yaail_start_pending_txn();
		s = yaail_txn_status(txn);
	} while(s == YAAIL_SCHEDULED || s == YAAIL_PENDING);
	return s;
}

void yaail_init_txn(yaail_txn_t * txn){
	uint8_t sreg = SREG;
	cli();
	*txn = YAAIL_DONE;
	SREG = sreg;
}

#include "yaail.h"

#include <util/twi.h>

enum yaail_status_e {
	YAAIL_NONE = 0,
	YAAIL_WAITING_START = 1,
	YAAIL_WAITING_DATA = 2,
	YAAIL_WAITING_REARM = 3,
	YAAIL_WAITING_RESTART = 4,
};

enum yaail_op_e {
	YAAIL_WRITE = 0,
	YAAIL_READ = 1,
	YAAIL_WRITE_AND_READ = 2,
};

struct yaail_t {
	enum yaail_status_e status;
	enum yaail_op_e op;
	uint8_t address;
	uint8_t *data;

	uint8_t length;
	uint8_t lengthRead;
	uint8_t done;

	bool rearmOK;
};

#if F_CPU==16000000L
uint8_t brr[2] = {18,10};
#else
#error "Unsupported F_CPU"
#endif

struct yaail_t il;


yaail_status_e yaail_init(yaail_baudrate_e br) {
	il.status = YAAIL_NONE;
	if (br >= YAAIL_NB_SUPPORTED_BAUDRATE) {
		return YAAIL_UNSUPPORTED_BAUDRATE;
	}
	TWBR = brr[br];
	il.rearmOK = true;
	return YAAIL_NO_ERROR;
}

void yaail_deinit() {
}

yaail_status_e yaail_read(uint8_t address, uint8_t * data, uint8_t length) {
	if ( il.status != YAAIL_NONE ) {
		return YAAIL_BUSY;
	}

	if ( (address & 0x80 ) != 0 ) {
		return YAAIL_INVALID_ADDRESS_ERROR;
	}
	il.address = (address << 1) | 0x01;
	il.data = data;
	il.length = length;
	TWCR = _BV(TWINT) | _BV(TWSTA) | _BV(TWEN);
	il.status = YAAIL_WAITING_START;
	il.rearmOK = false;
	il.op = YAAIL_READ;
	return YAAIL_NO_ERROR;
}

yaail_status_e yaail_write(uint8_t address, uint8_t * data, uint8_t length) {
	if ( il.status != YAAIL_NONE ) {
		return YAAIL_BUSY;
	}

	if ( (address & 0x80 ) != 0 ) {
		return YAAIL_INVALID_ADDRESS_ERROR;
	}
	il.address = address << 1;
	il.data = data;
	il.length = length;
	TWCR = _BV(TWINT) | _BV(TWSTA) | _BV(TWEN);
	il.status = YAAIL_WAITING_START;
	il.op = YAAIL_WRITE;
	il.rearmOK = false;
	il.op = YAAIL_WRITE;
	return YAAIL_NO_ERROR;
}

yaail_status_e yaail_write_and_read(uint8_t address, uint8_t * data, uint8_t lengthWrite, uint8_t lengthRead) {
	if ( il.status != YAAIL_NONE ) {
		return YAAIL_BUSY;
	}

	if ( (address & 0x80 ) != 0 ) {
		return YAAIL_INVALID_ADDRESS_ERROR;
	}
	il.address = address << 1;
	il.data = data;
	il.length = lengthWrite;
	il.lengthRead = lengthRead;
	TWCR = _BV(TWINT) | _BV(TWSTA) | _BV(TWEN);
	il.status = YAAIL_WAITING_START;
	il.rearmOK = false;
	il.op = YAAIL_WRITE_AND_READ;
	return YAAIL_NO_ERROR;
}


yaail_status_e yaail_poll() {
	if ( il.status == YAAIL_NONE ) {
		return YAAIL_NO_ERROR;
	}

	if ( il.status == YAAIL_WAITING_REARM ) {
		return YAAIL_DONE;
	}

	if ( (TWCR  & _BV(TWINT) ) == 0x00 ) {
		return YAAIL_BUSY;
	}

	uint8_t expected =0;
	switch(il.status) {
	case YAAIL_WAITING_START:
		if ( (TWSR & 0xf8) != TW_START && (TWSR &0xf8) != TW_REP_START ) {
			il.rearmOK = true;
			return YAAIL_START_ERROR;
		}
		il.done = 0xff;
		il.status = YAAIL_WAITING_DATA;
		TWDR = il.address;
		TWCR = _BV(TWINT) | _BV(TWEN) ;
		break;
	case YAAIL_WAITING_DATA:
		if ( (il.address & 0x01) != 0x00 ) {
			if (il.done == 0xff) {
				expected = TW_MR_SLA_ACK;
			} else {
				expected = TW_MR_DATA_ACK;
			}
		} else {
			if (il.done == 0xff) {
				expected = TW_MT_SLA_ACK;
			} else {
				expected = TW_MT_DATA_ACK;
			}
		}
		if ( expected != (TWSR & 0xf8) ) {
			il.rearmOK = true;
			TWCR = _BV(TWINT) | _BV(TWSTO) | _BV(TWEN);
			return YAAIL_ACK_ERROR;
		}
		if ( ++il.done >= il.length ) {
			if ( il.op == YAAIL_WRITE_AND_READ) {
				TWCR = _BV(TWINT) | _BV(TWEN) | _BV(TWSTA) ;
				il.address |= 0x01;
				il.status = YAAIL_WAITING_START;
				il.op = YAAIL_READ;
				il.length = il.lengthRead;
				return YAAIL_BUSY;
			} else {
				TWCR = _BV(TWINT) | _BV(TWEN) | _BV(TWSTO) ;
				il.rearmOK = true;
				il.status = YAAIL_WAITING_REARM;
				return YAAIL_DONE;
			}
		}
		if ( (il.address & 0x01 ) != 0x00 ) {
			il.data[il.done] = TWDR;
		} else {
			TWDR = il.data[il.done];
		}
		TWCR = _BV(TWINT) | _BV(TWEN);
		break;
	default:
		break;
	}
	return YAAIL_BUSY;
}



yaail_status_e yaail_rearm() {
	if (il.rearmOK == false) {
		return YAAIL_BUSY;
	}
	il.status= YAAIL_NONE;
	return YAAIL_NO_ERROR;
}

#pragma once

#include "inttypes.h"
#include "stdbool.h"


typedef enum {
	YAAIL_166 = 0,
	YAAIL_100,
	YAAIL_50,
	YAAIL_25,
	YAAIL_NB_SUPPORTED_BAUDRATE
} yaail_baudrate_e;


typedef enum {
	YAAIL_NO_ERROR = 0,
	YAAIL_INVALID_LENGTH_ERROR = 1,
	YAAIL_INVALID_ADDRESS_ERROR = 2,
	YAAIL_BUFFER_OVERFLOW_ERROR = 3,
	YAAIL_UNSUPPORTED_BAUDRATE_ERROR = 4
} yaail_error_e;



typedef enum {
	YAAIL_SCHEDULED = 0,
	YAAIL_PENDING = 1,
	YAAIL_DONE = 2,
	YAAIL_START_ERROR = 3,
	YAAIL_SLA_ACK_ERROR = 4,
	YAAIL_DATA_ACK_ERROR = 5
} yaail_txn_status_e;

typedef volatile yaail_txn_status_e yaail_txn_t;


#define yaail_txn_status_has_error(txn_status) ( (txn_status) > YAAIL_DONE )

yaail_error_e yaail_init(yaail_baudrate_e br);

void yaail_init_txn(yaail_txn_t * txn);

yaail_error_e yaail_read(yaail_txn_t * txn,
                         uint8_t address,
                         uint8_t * buffer,
                         uint8_t length);

yaail_error_e yaail_write(yaail_txn_t * txn,
                          uint8_t address,
                          const uint8_t * buffer,
                          uint8_t length);

yaail_error_e yaail_write_and_read(yaail_txn_t * txn,
                                   uint8_t address,
                                   uint8_t * data,
                                   uint8_t lengthWrite,
                                   uint8_t lengthRead);

yaail_txn_status_e yaail_txn_status(const yaail_txn_t *);

yaail_txn_status_e yaail_spin_until_done(const yaail_txn_t *);

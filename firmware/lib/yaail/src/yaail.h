#pragma once

#include "inttypes.h"
#include "stdbool.h"


typedef enum {
	YAAIL_100 = 0,
	YAAIL_166,
	YAAIL_NB_SUPPORTED_BAUDRATE
} yaail_baudrate_e;

typedef enum {
	YAAIL_NO_ERROR = 0,
	YAAIL_DONE = 1,
	YAAIL_UNSUPPORTED_BAUDRATE,
	YAAIL_BUSY,
	YAAIL_INVALID_ADDRESS_ERROR,
	YAAIL_START_ERROR,
	YAAIL_ACK_ERROR
} yaail_status_e;


yaail_status_e yaail_init(yaail_baudrate_e br);

void yaail_deinit();

yaail_status_e yaail_read(uint8_t address, uint8_t * buffer, uint8_t length);

yaail_status_e yaail_write(uint8_t address, uint8_t * buffer, uint8_t length);

yaail_status_e yaail_write_and_read(uint8_t address, uint8_t * data, uint8_t lengthWrite, uint8_t lengthRead);

yaail_status_e yaail_poll();

yaail_status_e yaail_rearm();

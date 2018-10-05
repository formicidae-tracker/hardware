#pragma once


#include "config.h"
#include "txn.h"

typedef enum {
	YACCL_BR_100 = 0,
	YACCL_BR_125,
	YACCL_BR_200,
	YACCL_BR_250,
	YACCL_BR_500,
	YACCL_BR_1000,
	YACCL_NB_SUPPORTED_BAUDRATE
} yaccl_baudrate_e;


typedef struct {
	yaccl_baudrate_e baudrate;
} yaccl_config_t;


typedef enum {
	YACCL_NO_ERROR  = 0,
	YACCL_GENERIC_ERROR=1,
	YACCL_ERR_INVALID_BAUDRATE,
	YACCL_ERR_TX_DATA_OVERFLOW,
	YACCL_ERR_MOB_OVERFLOW,
} yaccl_error_e;

yaccl_error_e yaccl_init(yaccl_config_t config);

yaccl_error_e  yaccl_send(yaccl_idt_t target, const uint8_t * data, uint8_t length, yaccl_txn_t * txn);

yaccl_error_e yaccl_listen(yaccl_idt_t from, yaccl_idt_t mask, uint8_t * data, yaccl_txn_t * txn);

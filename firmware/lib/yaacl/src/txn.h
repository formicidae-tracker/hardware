#pragma once

#include "idt.h"

typedef enum {
	YACCL_TXN_PENDING,
	YACCL_TXN_COMPLETED,
	YACCL_TXN_HAD_ERROR,
} yaccl_txn_status_e;

typedef enum {
	YACCL_TXN_NO_ERROR = 0,
	YACCL_TXN_ERR_UNSPECIFIED =1
} yaccl_txn_error_e;


typedef struct s_yaccl_txn_t {
	yaccl_idt_t ID;
	//this field should be considered opaque
	uint8_t MobID;
	uint8_t length;
	uint8_t * data;
} yaccl_txn_t;


//return transaction status
yaccl_txn_status_e yaccl_txn_status(const yaccl_txn_t * txn);

// return actual data length
uint8_t yaccl_txn_data_length(const yaccl_txn_t * txn);

//return any pending error
yaccl_txn_error_e yaccl_txn_get_error(const yaccl_txn_t * txn);

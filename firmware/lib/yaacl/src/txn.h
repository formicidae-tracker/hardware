#pragma once

#include "idt.h"

// Represents the status of a transaction <yaacl_txn_t>
typedef enum yaacl_txn_status_e {
	// Transaction is not yet submitted
	YAACL_TXN_UNSUBMITTED = 0,
	// Transaction is submitted and waiting to complete
	YAACL_TXN_PENDING = 1,
	// Transaction is completed.
	YAACL_TXN_COMPLETED = 2,
	// Transaction had an unspecified error (to be removed)
	YAACL_TXN_ACK_ERROR = (1 << 2),
	YAACL_TXN_FORM_ERROR = (1 << 3),
	YAACL_TXN_CRC_ERROR = (1 << 4),
	YAACL_TXN_STUFFING_ERROR = (1 << 5),
	YAACL_TXN_BIT_ERROR = (1 << 6),
	// Rx buffer was too small
	YAACL_TXN_ERR_RX_OVERFLOW = (1 << 7)
} yaacl_txn_status_e;

#define yaacl_txn_had_error(status) ((status) > YAACL_TXN_COMPLETED)

// Represents a transaction on the CAN bus
//
// <yaacl_txn_t> represents a transmission (reception or transmission
// on the can bus). A <yaacl_txn_t> is submitted to the CAN module
// with the helps of submission functions <yaacl_send> and <yaacl_listen>.
typedef struct s_yaacl_txn_t {
	// The ID of the transaction.
	//
	// Need to be set both for sending and receiving. Will be modified
	// by the actual received ID when receiving a message.
	yaacl_idt_t ID;
	// The mask for reception
	//
	// Mask for the reception of the message.
	yaacl_idt_t mask;
	// The length of the data
	//
	// Need to be set to a value between 0 and 8 for transmission and
	// will be written to the correct value for a reception
	uint8_t     length;
	// Pointer for a buffer for the data of the transaction
	//
	// Need to be set to a valid memory location. Will be read for TX
	// and written by a reception.
	uint8_t     * data;

	// INTERNAL FIELD
	uint8_t MobID;

} yaacl_txn_t;


// Polls for the actual status of the <yaacl_txn>.
// @txn the previously submitted <yaacl_txn_t>
//
// This functions polls for the current status of that transaction.
//
// @return the <yaacl_txn_status_e> of the transaction
yaacl_txn_status_e yaacl_txn_status(yaacl_txn_t * txn);

void yaacl_init_txn(yaacl_txn_t *);

//void yaacl_txn_clear(yaacl_txn_t * txn);

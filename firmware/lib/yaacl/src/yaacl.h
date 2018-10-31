#pragma once

#include "config.h"
#include "txn.h"


// Available CAN baudrate selection
typedef enum yaacl_baudrate_e {
	// 100 kbit/s
	YAACL_BR_100 = 0,
	// 125 kbit/s
	YAACL_BR_125,
	// 200 kbit/s
	YAACL_BR_200,
	// 250 kbit/s
	YAACL_BR_250,
	// 500 kbit/s
	YAACL_BR_500,
	// 1 Mbit/s
	YAACL_BR_1000,
	// Number of supported baudrates
	YAACL_NB_SUPPORTED_BAUDRATE
} yaacl_baudrate_e;

// Available configuration of the CAN module in yaacl.
typedef struct yaacl_config_t {
	// The wanted baudrate
	yaacl_baudrate_e baudrate;
} yaacl_config_t;



// General error that can be raised by yaacl
typedef enum yaacl_error_e{
	// 0 means no error
	YAACL_NO_ERROR  = 0,
	// Generic unpsecified error
	YAACL_GENERIC_ERROR=1,
	// Invalid baudrate setting
	YAACL_ERR_INVALID_BAUDRATE,
	// Attempt to transmit too much bit in a single transaction
	YAACL_ERR_TX_DATA_OVERFLOW,
	// No more message object available.
	YAACL_ERR_MOB_OVERFLOW,
} yaacl_error_e;


// Inits yaacl and enable the CAN Module.
yaacl_error_e yaacl_init(const yaacl_config_t *config);


// Stops the module
yaacl_error_e yaacl_deinit();

/* Performs a transmission on the CAN bus.
 *  @txn a pre-filled <yaacl_txn_t> structure with the
 *       <yaacl_txn_t::ID> <yaacl_txn_t::length> and
 *       <yaacl_txn_t::data> fields set.
 *
 *  @return
 * * YAACL_NO_ERROR if submission is succesful.
 * * YACLL_ERR_MOB_OVERFLOW if all CAN message object are currently used.
 * * YAACL_ERR_TX_DATA_OVERFLOW if the specified length is bigger than 8.
 */
yaacl_error_e  yaacl_send(yaacl_txn_t * txn);



/* Performs a receptions on the CAN bus.
 *  @txn a pre-filled <yaacl_txn_t> structure with the
 *       <yaacl_txn_t::ID>, <yaacl_txn_t::mask>,
 *       <yaacl_txn_t::length>, and <yaacl_txn_t::data> fields set.
 *
 *  @return
 * * YAACL_NO_ERROR if submission is succesful.
 * * YAACL_ERR_MOB_OVERFLOW if all CAN message object are currently used.
 */
yaacl_error_e yaacl_listen(yaacl_txn_t * txn);

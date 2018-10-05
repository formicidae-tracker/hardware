#include "txn.h"


yaccl_txn_status_e yaccl_txn_status(const yaccl_txn_t * txn) {
	//NYI
	return YACCL_TXN_PENDING;
}

// return actual data length
uint8_t yaccl_txn_data_length(const yaccl_txn_t * txn) {
	return txn->length;
}

//return any pending error
yaccl_txn_error_e yaccl_txn_get_error(const yaccl_txn_t * txn) {
	return YACCL_TXN_ERR_UNSPECIFIED;
}

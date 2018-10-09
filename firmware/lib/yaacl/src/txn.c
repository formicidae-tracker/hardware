#include "txn.h"

#include "config.h"

void yaacl_init_txn(yaacl_txn_t * txn) {
	//mark the txn as UNSUBMITTED;
	txn->MobID = YAACL_NB_MOB;
}

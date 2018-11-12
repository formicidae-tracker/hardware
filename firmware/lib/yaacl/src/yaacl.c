#include "yaacl.h"
#include "config.h"

#include <avr/io.h>

#define yaacl_can_reset() do{	  \
		CANGCON |= _BV(SWRES); \
	}while(0)

#define yaacl_can_enable() do{	  \
		CANGCON |= _BV(ENASTB); \
	}while(0)

#define yaacl_can_disable() do{	  \
		CANGCON &= ~(_BV(ENASTB)); \
	}while(0)

#define yaacl_select_mob(mob) do {	  \
		CANPAGE = ((mob) & YAACL_MOB_MASK) << 4; \
	}while(0)

#define yaacl_mob_disable() do {	\
		CANCDMOB &= ~(_BV(CONMOB0) | _BV(CONMOB1)); \
	}while(0)

#define yaacl_mob_clear_status() do {	  \
	CANSTMOB &= 0x80; \
	}while(0)

#define yaacl_mob_rearm() do {	  \
		yaacl_mob_disable(); \
		yaacl_mob_clear_status(); \
	}while(0)


#define yaacl_clear_mob() do{ \
		CANSTMOB = 0x0; \
		CANCDMOB = 0x0; \
		CANIDT1 = 0x0; \
		CANIDT2 = 0x0; \
		CANIDT3 = 0x0; \
		CANIDT4= 0x0; \
		CANIDM1 = 0x0; \
		CANIDM2 = 0x0; \
		CANIDM3 = 0x0; \
		CANIDM4 = 0x0; \
		CANSTMH = 0x0; \
		CANSTML = 0x0; \
}while(0);

#define yaacl_std_idt_03_00(idt) (((*((uint8_t*)(&(idt)) + 0)) << 5 ))
#define yaacl_std_idt_10_04(idt) (((*((uint8_t*)(&(idt)) + 1)) << 5 ) | ((*((uint8_t*)(&(idt)) + 0)) >> 3 ))
#define yaacl_ext_idt_04_00(idt) (((*((uint8_t*)(&(idt)) + 0)) << 3 ))
#define yaacl_ext_idt_12_05(idt) (((*((uint8_t*)(&(idt)) + 1)) << 3 ) | ((*((uint8_t*)(&(idt)) + 0)) >> 5 ))
#define yaacl_ext_idt_20_13(idt) (((*((uint8_t*)(&(idt)) + 2)) << 3 ) | ((*((uint8_t*)(&(idt)) + 1)) >> 5 ))
#define yaacl_ext_idt_28_21(idt) (((*((uint8_t*)(&(idt)) + 3)) << 3 ) | ((*((uint8_t*)(&(idt)) + 2)) >> 5 ))


// we mask outgoing ID to always send R0 as 0 (dominant)
#define yaacl_set_mob_idt_std(idt) do {	  \
		if ( yaacl_idt_test_rtrbit(idt) ) { \
			CANIDT4 = _BV(RTRTAG); \
		} else { \
			CANIDT4 = 0x00; \
		} \
		CANIDT3 = 0x0; \
		CANIDT2 = yaacl_std_idt_03_00(idt); \
		CANIDT1 = yaacl_std_idt_10_04(idt); \
		CANCDMOB &= ~(_BV(IDE)); \
	}while(0)

// we mask outgoing ID to always send R0,R1 as 0 (dominant)
#define yaacl_set_mob_idt_ext(idt) do {	  \
		CANIDT4 = yaacl_ext_idt_04_00(idt); \
		if ( yaacl_idt_test_rtrbit(idt) ) { \
			CANIDT4 |= _BV(RTRTAG); \
		} \
		CANIDT3 = yaacl_ext_idt_12_05(idt); \
		CANIDT2 = yaacl_ext_idt_20_13(idt); \
		CANIDT1 = yaacl_ext_idt_28_21(idt); \
		CANCDMOB |= _BV(IDE); \
	}while(0)


#define yaacl_set_mob_mask_std(idt) do {	  \
		CANIDM4 = 0x00; \
		if( yaacl_idt_test_rtrbit(idt) ) { \
			CANIDM4 |= _BV(RTRMSK); \
		} \
		if ( yaacl_idt_test_idebit(idt) ) { \
			CANIDM3 |= _BV(IDEMSK); \
		} \
		CANIDM3 = 0x00; \
		CANIDM2 = yaacl_std_idt_03_00(idt); \
		CANIDM1 = yaacl_std_idt_10_04(idt); \
	}while(0)

#define yaacl_set_mob_mask_ext(idt) do {	  \
		CANIDM4 = yaacl_ext_idt_04_00(idt); \
		if( yaacl_idt_test_rtrbit(idt) ) { \
			CANIDM4 |= _BV(RTRMSK); \
		} \
		if ( yaacl_idt_test_idebit(idt) ) { \
			CANIDM3 |= _BV(IDEMSK); \
		} \
		CANIDM3 = yaacl_ext_idt_12_05(idt); \
		CANIDM2 = yaacl_ext_idt_20_13(idt); \
		CANIDM1 = yaacl_ext_idt_28_21(idt); \
	}while(0)

#define yaacl_get_mob_idt_std(idt) do {	  \
		if ( (CANIDT4 & _BV(RTRTAG) ) != 0 ) { \
			*((uint8_t*)(&(idt)) + 3) = YAACL_RTRBIT_MSK >> 24; \
		} else { \
			*((uint8_t*)(&(idt)) + 3) = 0x0; \
		} \
		*((uint8_t*)(&(idt)) + 2) = 0; \
		*((uint8_t*)(&(idt)) + 1) = CANIDT1 >>  5; \
		*((uint8_t*)(&(idt)) + 0) = (CANIDT2 >>  5) | (CANIDT1 << 3); \
	}while(0);

#define yaacl_get_mob_idt_ext(idt) do {	  \
		*((uint8_t*)(&(idt)) + 3) = CANIDT1 >> 3 | (YAACL_IDEBIT_MSK >> 24) ; \
		if ( (CANIDT4 & _BV(RTRTAG) ) != 0 ) { \
			*((uint8_t*)(&(idt)) + 3) |= YAACL_RTRBIT_MSK >> 24; \
		} \
		*((uint8_t*)(&(idt)) + 2) = CANIDT1 << 5 | CANIDT2 >> 3; \
		*((uint8_t*)(&(idt)) + 1) = CANIDT2 << 5 | CANIDT3 >> 3; \
		*((uint8_t*)(&(idt)) + 0) = CANIDT3 << 5 | CANIDT4 >> 3; \
	}while(0);

#define yaacl_set_mob_dlc(dlc) do {	  \
		CANCDMOB = (CANCDMOB & 0xf0) |  ((dlc) & 0x0f); \
	}while(0)


#define yaacl_enable_mob_tx() do { \
		CANCDMOB |= _BV(CONMOB0); \
	}while(0)

#define yaacl_enable_mob_rx() do { \
		CANCDMOB |= _BV(CONMOB1); \
	}while(0)

uint8_t yaacl_baudrate_config[18] =
#if F_CPU==16000000
	{
	0x12,0x0c,0x37, // 100
	0x0e,0x0c,0x37, // 125
	0x08,0x0c,0x37, // 200
	0x06,0x0c,0x37, // 250
	0x02,0x0c,0x37, // 500
	0x02,0x04,0x13, // 1000
	};
#elif F_CPU==12000000
	{
	0x0a,0x0e,0x4b, // 100
	0x0a,0x0c,0x37, // 125
	0x04,0x0e,0x4b, // 200
	0x04,0x0c,0x37, // 250
	0x02,0x08,0x25, // 500
	0x00,0x08,0x24, // 1000
	};
#elif F_CPU==8000000
	{
	0x08,0x0c,0x37, // 100
	0x06,0x0c,0x37, // 125
	0x02,0x0e,0x4b, // 200
	0x02,0x0c,0x37, // 250
	0x02,0x04,0x13, // 500
	0x00,0x04,0x12, // 1000
	};
#else
#error "Yaacl only supports 16,12 and 8 MHz clock rate"
#endif



yaacl_error_e yaacl_set_baudrate(yaacl_baudrate_e br) {
	if (br >= YAACL_NB_SUPPORTED_BAUDRATE ) {
		return YAACL_ERR_INVALID_BAUDRATE;
	}

	CANBT1 = yaacl_baudrate_config[br*3];
	CANBT2 = yaacl_baudrate_config[br*3+1];
	CANBT3 = yaacl_baudrate_config[br*3+2];
	return YAACL_NO_ERROR;
}


yaacl_error_e yaacl_init(const yaacl_config_t * config) {
	yaacl_error_e err = yaacl_set_baudrate(config->baudrate);
	if ( err != YAACL_NO_ERROR) {
		return err;
	}

	for (uint8_t i = 0; i < YAACL_NB_MOB; i++) {
		yaacl_select_mob(i);
		yaacl_clear_mob();
	}

	yaacl_can_enable();

	return YAACL_NO_ERROR;
}


uint8_t yaacl_next_free_mob() {
	uint8_t page_saved = CANPAGE;
	for (uint8_t i = 0; i < YAACL_NB_MOB; i++) {
		yaacl_select_mob(i);
		// check if mob is disabled
		if ( ( CANCDMOB & 0xC0 ) == 0x00 ) {
			CANPAGE = page_saved;
			return i;
		}
	}
	CANPAGE = page_saved;
	return 0xff;
}


yaacl_error_e  yaacl_send(yaacl_txn_t * txn) {
	uint8_t next_free = yaacl_next_free_mob();
	if (next_free == 0xff) {
		txn->MobID = YAACL_NB_MOB;
		return YAACL_ERR_MOB_OVERFLOW;
	}
	if (txn->length > 8 ) {
		txn->MobID = YAACL_NB_MOB;
		return YAACL_ERR_TX_DATA_OVERFLOW;
	}

	txn->MobID = next_free;
	yaacl_select_mob(next_free);
	if ( yaacl_idt_test_idebit(txn->ID) ) {
		yaacl_set_mob_idt_ext(txn->ID);
	} else {
		yaacl_set_mob_idt_std(txn->ID);
	}
	yaacl_set_mob_dlc(txn->length);
	//setting the data to send
	for (uint8_t i = 0; i < txn->length; i++ ) {
		CANMSG = *(txn->data + i);
	}
	yaacl_enable_mob_tx();

	return YAACL_GENERIC_ERROR;
}

yaacl_error_e yaacl_listen(yaacl_txn_t * txn) {
	uint8_t next_free = yaacl_next_free_mob();
	if (next_free == 0xff) {
		txn->MobID = YAACL_NB_MOB;
		return YAACL_ERR_MOB_OVERFLOW;
	}

	txn->MobID = next_free;
	yaacl_select_mob(next_free);
	if ( yaacl_idt_test_idebit(txn->ID) ) {
		yaacl_set_mob_idt_ext(txn->ID);
		yaacl_set_mob_mask_ext(txn->mask);
	} else {
		yaacl_set_mob_idt_std(txn->ID);
		yaacl_set_mob_mask_std(txn->mask);
	}

	yaacl_enable_mob_rx();


	return YAACL_GENERIC_ERROR;
}



yaacl_txn_status_e yaacl_txn_status(yaacl_txn_t * txn) {
	if (txn->MobID >= YAACL_NB_MOB ) {
		return YAACL_TXN_UNSUBMITTED;
	}

	yaacl_select_mob(txn->MobID);
	uint8_t mob_command = CANCDMOB;
	if ( (mob_command & (_BV(CONMOB1) | _BV(CONMOB0))) == 0x00 ) {
		// haven't be submitted yet !
		return YAACL_TXN_UNSUBMITTED;
	}

	//test the status
	uint8_t mob_status = CANSTMOB;

	if ( (mob_status & 0x7f) == 0x00 ) {
		return YAACL_TXN_PENDING;
	}

	// in any case return should yaacl_mob_ream();

	if ( mob_status & _BV(TXOK) ) {
		yaacl_mob_rearm();
		txn->MobID = YAACL_NB_MOB;
		return YAACL_TXN_COMPLETED;
	}

	if ( (mob_status & 0x3f) == _BV(RXOK) ) {
		// readout the received ID
		mob_command = CANCDMOB;
		if ( (mob_command & _BV(IDE)) != 0 ) {
			yaacl_get_mob_idt_ext(txn->ID);
		} else {
			yaacl_get_mob_idt_std(txn->ID);
		}

		uint8_t actual_size = mob_command & 0x0f;
		if ( (mob_status & _BV(DLCW)) != 0
		     && actual_size > txn->length) {
			yaacl_mob_rearm();
			txn->MobID = YAACL_NB_MOB;
			return YAACL_TXN_ERR_RX_OVERFLOW;
		}
		txn->length = actual_size;
		for (uint8_t i = 0;  i < actual_size; i++) {
			*(txn->data+i) = CANMSG;
		}
		yaacl_mob_rearm();
		txn->MobID = YAACL_NB_MOB;
		return YAACL_TXN_COMPLETED;
	}
	// so we get any error, we re-arm

	yaacl_mob_rearm();
	txn->MobID = YAACL_NB_MOB;
	return (mob_status & 0x1f) << 2;
}


yaacl_error_e yaacl_deinit() {
	yaacl_can_disable();
	return YAACL_NO_ERROR;
}

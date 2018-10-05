#include "yaccl.h"

#include <avr/io.h>

#define yaccl_can_reset() do{	  \
		CANGCON |= _BV(SWRES); \
	}while(0)

#define yaccl_can_enable() do{	  \
		CANGCON |= _BV(ENASTB); \
	}while(0)

#define yaccl_can_disable() do{	  \
		CANGCON &= ~(_BV(ENASTB)); \
	}while(0)

#define yaccl_select_mob(mob) do {	  \
		CANPAGE = ((mob) & YACCL_MOB_MASK) << 4; \
	}while(0)

#define yaccl_clear_mob() do{ \
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


#define yaccl_set_mob_idt(idt) do {\
		CANIDT1 = (idt).data[0]; \
		CANIDT2 = (idt).data[1]; \
		CANIDT3 = (idt).data[2]; \
		CANIDT4 = (idt).data[3] & 0xfc; \
	}while(0)

#define yaccl_set_mob_mask(idt) do {\
		CANIDT1 = (idt).data[0]; \
		CANIDT2 = (idt).data[1]; \
		CANIDT3 = (idt).data[2]; \
		CANIDT4 = ((idt).data[3] & 0xfc) | 1; \
	}while(0)

#define yaccl_set_mob_dlc(dlc) do {	  \
		CANCDMOB |= (dlc) & 0x0f; \
	}while(0)


#define yaccl_enable_mob_tx() do { \
		CANCDMOB |= _BV(CONMOB0); \
	}while(0)

#define yaccl_enable_mob_rx() do { \
		CANCDMOB |= _BV(CONMOB1); \
	}while(0)

uint8_t yaccl_baudrate_config[18] =
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
#error "Yaccl only supports 16,12 and 8 MHz clock rate"
#endif



yaccl_error_e yaccl_set_baudrate(yaccl_baudrate_e br) {
	if (br >= YACCL_NB_SUPPORTED_BAUDRATE ) {
		return YACCL_ERR_INVALID_BAUDRATE;
	}

	CANBT1 = yaccl_baudrate_config[br*3];
	CANBT2 = yaccl_baudrate_config[br*3+1];
	CANBT3 = yaccl_baudrate_config[br*3+2];

	return YACCL_NO_ERROR;
}


yaccl_error_e yaccl_init(yaccl_config_t config) {
	yaccl_error_e err = yaccl_set_baudrate(config.baudrate);
	if ( err != YACCL_NO_ERROR) {
		return err;
	}

	for (uint8_t i = 0; i < YACCL_NB_MOB; i++) {
		yaccl_select_mob(i);
		yaccl_clear_mob();
	}

	yaccl_can_enable();

	return YACCL_NO_ERROR;
}


uint8_t yaccl_next_free_mob() {
	uint8_t page_saved = CANPAGE;
	for (uint8_t i = 0; i < YACCL_NB_MOB; i++) {
		yaccl_select_mob(i);
		// check if mob is disabled
		if ( ( CANCDMOB & 0xC0 ) == 0x00 ) {
			CANPAGE = page_saved;
			return i;
		}
	}
	CANPAGE = page_saved;
	return 0xff;
}


yaccl_error_e  yaccl_send(yaccl_idt_t target, const uint8_t * data, uint8_t length, yaccl_txn_t * txn) {
	uint8_t next_free = yaccl_next_free_mob();
	if (next_free == 0xff) {
		return YACCL_ERR_MOB_OVERFLOW;
	}
	if (length > 8 ) {
		return YACCL_ERR_TX_DATA_OVERFLOW;
	}

	txn->MobID = next_free;
	yaccl_select_mob(next_free);
	yaccl_set_mob_idt(target);
	//setting the data to send
	for (uint8_t i = 0; i < length; i++ ) {
		CANMSG = *(data + i);
	}

	yaccl_set_mob_dlc(length);
	yaccl_enable_mob_tx();

	return YACCL_GENERIC_ERROR;
}

yaccl_error_e yaccl_listen(yaccl_idt_t from, yaccl_idt_t mask, uint8_t * data, yaccl_txn_t * txn) {
	uint8_t next_free = yaccl_next_free_mob();
	if (next_free == 0xff) {
		return YACCL_ERR_MOB_OVERFLOW;
	}

	txn->MobID = next_free;
	txn->data = data;
	yaccl_select_mob(next_free);
	yaccl_set_mob_idt(from);
	yaccl_set_mob_mask(mask);
	yaccl_enable_mob_rx();

	return YACCL_GENERIC_ERROR;
}

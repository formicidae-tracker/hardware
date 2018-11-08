#include <arke-avr.h>

#include "LEDs.h"
#include "Systime.h"

#include <yaacl.h>

int main() {
	InitSystime();
	InitLEDs();

	Systime_t last = 0;

	yaacl_config_t conf;
	conf.baudrate = YAACL_BR_100;
	yaacl_init(&conf);

	yaacl_txn_t rx,tx[2];
	uint8_t data[8];

	yaacl_init_txn(&rx);
	yaacl_init_txn(&tx[0]);
	yaacl_init_txn(&tx[1]);

	yaacl_make_ext_idt(rx.ID,0,0);
	yaacl_make_ext_mask(rx.mask,0,0,0);
	rx.data = data;
	rx.length = 8;

	yaacl_make_std_idt(tx[0].ID,0x123,0);
	tx[0].data = data;
	tx[0].length = 0;

	yaacl_make_ext_idt(tx[1].ID,0x12345678,0);
	tx[1].data = data;
	tx[1].length = 0;

	yaacl_listen(&rx);
	uint8_t nextSend = 0;
	while(true) {
		ProcessLEDs();
		Systime_t now = GetSystime();
		if ( (now-last) >= 200 ) {
			last = now;
			if ( yaacl_txn_status(&tx[nextSend]) == YAACL_TXN_UNSUBMITTED ) {
				yaacl_send(&tx[nextSend]);
				nextSend = (nextSend + 1) & 0x01;
			}
		}


		yaacl_txn_status_e s = yaacl_txn_status(&rx);
		if (s != YAACL_TXN_PENDING) {
			LEDReadyToggle();
			//			LEDErrorToggle();
			//discard data and lsiten again.
			rx.length = 8;
			yaacl_listen(&rx);
		}
		for (uint8_t i = 0; i < 2; ++i) {
			s = yaacl_txn_status(&tx[i]);
			if (s == YAACL_TXN_COMPLETED) {
				LEDErrorOff();
			} else if ( yaacl_txn_had_error(s) ) {
				LEDErrorOn();
			}
		}

	}
}

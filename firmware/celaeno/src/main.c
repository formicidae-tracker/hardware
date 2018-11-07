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

	yaacl_txn_t rx,tx;
	uint8_t data[8];

	yaacl_make_ext_idt(rx.ID,0,0);
	yaacl_make_ext_mask(rx.mask,0,0,0);
	rx.data = data;
	rx.length = 8;

	yaacl_make_std_idt(tx.ID,0x123,0);
	tx.data = data;
	tx.length = 0;

	yaacl_send(&tx);
	yaacl_listen(&rx);
	while(true) {
		ProcessLEDs();
		Systime_t now = GetSystime();
		if ( (now-last) >= 1000 ) {
			last = now;
			if ( yaacl_txn_status(&tx) == YAACL_TXN_UNSUBMITTED ) {
				yaacl_send(&tx);
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
		s = yaacl_txn_status(&tx);
		if (s == YAACL_TXN_COMPLETED) {
			LEDErrorOff();
		} else if ( yaacl_txn_had_error(s) ) {
			LEDErrorOn();
		}


	}
}

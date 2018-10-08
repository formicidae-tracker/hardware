#include "Systime.h"
#include "LEDs.h"

#include <yaacl.h>

int main() {
	InitLEDs();
	InitSystime();


	yaacl_config_t config;
	config.baudrate = YAACL_BR_500;
	yaacl_init(&config);


	yaacl_txn_t rx[2];
	uint8_t data[16];
	for (unsigned int i = 0; i < 2; i++){
		// listen to absolutely everything, from standard to extended
		// frames.
		yaacl_make_ext_idt(rx[i].ID,0x0,0);
		yaacl_make_ext_mask(rx[i].mask,0x0,0,0);
		rx[i].data = &(data[i*8]);
		rx[i].length = 8;
		yaacl_listen(&(rx[i]));
	}

	Systime_t last = 0;

	while(1) {
		for ( unsigned int i = 0; i < 2; i++ ) {
			// see if we got some nice message ongoing.
			yaacl_txn_status_e status = yaacl_txn_status(rx+i);
			if (status == YAACL_TXN_PENDING) {
				continue;
			}
			if (status == YAACL_TXN_COMPLETED ) {
				//TODO send to host
			}
			if ( yaacl_txn_had_error(status) ) {
				//TODO : maybe report error count ?
				LEDErrorOn();
			}
			// restarts the listen loop on the message.
			yaacl_listen(rx+i);
		}


		Systime_t now = GetSystime();

		if ( (now-last) >= 500 ) {
			LEDReadyToggle();
			last = now;
		}

	}

	return 0;
}

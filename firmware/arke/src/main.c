#include "Systime.h"
#include "LEDs.h"
#include "HostLink.h"


#include <yaacl.h>

int main() {
	InitLEDs();
	InitSystime();
	InitHostLink();

	yaacl_config_t config;
	config.baudrate = YAACL_BR_500;
	yaacl_init(&config);


	yaacl_txn_t rx[2];
	uint8_t data[16];
	for (unsigned int i = 0; i < 2; i++){
		// listen to absolutely everything, both standard to extended
		// frames.
		yaacl_make_ext_idt(rx[i].ID,0x0,0);
		rx[i].mask = 0x00; // get absolutely everything
		rx[i].data = &(data[i*8]);
		rx[i].length = 8;
		yaacl_listen(&(rx[i]));
	}

	while(1) {
		for ( unsigned int i = 0; i < 2; i++ ) {
			// see if we got some nice message ongoing.
			yaacl_txn_status_e status = yaacl_txn_status(rx+i);
			if (status == YAACL_TXN_PENDING) {
				continue;
			}
			if (status == YAACL_TXN_COMPLETED ) {
				HostSendCANPacket(rx+i);
			}
			if ( yaacl_txn_had_error(status) ) {
				HostReportCANRxError();
				LEDErrorOn();
			}
			//re-listen to everything, mask is still zero, and length
			//has to be reset.
			rx[i].length = 8;
			yaacl_listen(rx+i);
		}

		ProcessHostLink();
		ProcessLEDs();


	}

	return 0;
}

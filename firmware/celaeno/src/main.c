#include <arke-avr.h>

implements_ArkeSoftwareReset()

int main() {
	InitArke();
	ArkeSystime_t last = 0;
	yaacl_txn_t toHelios;
	yaacl_init_txn(&toHelios);
	ArkeHeliosSetPoint heliosValue;
	int8_t increment = 1;
	heliosValue.Visible = 0;
	heliosValue.UV = 0;
	uint8_t sendRequest = 0;
	for(;;) {
		ArkeProcess();

		ArkeSystime_t now  = ArkeGetSystime();
		if ( (now-last) >= 50 ) {
			if ( heliosValue.Visible == 0 ) {
				increment = 1;
			} else if ( heliosValue.Visible == 255 ) {
				increment = -1;
			}
			heliosValue.Visible += increment;
			sendRequest = 1;

			last = now;
		}

		yaacl_txn_status_e status = yaacl_txn_status(&toHelios);
		if ( status == YAACL_TXN_PENDING || sendRequest == 0) {
			continue;
		}

		yaacl_error_e err = ArkeSendHeliosSetPoint(&toHelios,false,false,0,&heliosValue);
		if ( err ==  YAACL_NO_ERROR ) {
			sendRequest = 0;
		}
	}
	return 0;
}

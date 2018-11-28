#include "LEDs.h"
#include "Sensors.h"

#include <arke-avr.h>

#include <yaail.h>


implements_ArkeSoftwareReset()

typedef struct {
	yaacl_txn_t report;
	uint8_t inBuffer[8];
} Zeus_t;

Zeus_t Z;

int main() {
	InitArke(Z.inBuffer,8);
	InitLEDs();
	InitSensors();


	LEDReadyPulse();
	yaail_init(YAAIL_25);
	yaacl_init_txn(&Z.report);

	while(true) {
		ProcessLEDs();
		uint8_t dlc;
		ArkeProcess(&dlc);
		bool hasNew = ProcessSensors();
		if ( hasNew && yaacl_txn_status(&Z.report) != YAACL_TXN_PENDING) {
			ArkeSendZeusReport(&Z.report,false,GetSensorData());
		}

	}

	return 0;
}

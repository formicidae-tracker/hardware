#include "LEDs.h"
#include "Sensors.h"
#include "FanControl.h"
#include "Heaters.h"
#include "ClimateController.h"

#include <arke-avr.h>
#include <yaail.h>


implements_ArkeSoftwareReset()

typedef struct {
	yaacl_txn_t report,txSetPoint,status;
	ArkeSystime_t reportPeriod,lastReport;
	union {
		ArkeZeusSetPoint sp;
		ArkeZeusConfig config;
		uint8_t bytes[8];
	} inBuffer;
} Zeus_t;

Zeus_t Z;


void InitZeus() {
	InitArke(Z.inBuffer.bytes,8);
	Z.reportPeriod = 0;
	Z.lastReport = 0;

	LEDReadyPulse();
	yaail_init(YAAIL_25);
	yaacl_init_txn(&Z.report);
	yaacl_init_txn(&Z.txSetPoint);
	yaacl_init_txn(&Z.status);

}


void ProcessIncoming() {
	uint8_t dlc;
	yaacl_idt_t res = ArkeProcess(&dlc);
	bool rtr = yaacl_idt_test_rtrbit(res);
	ArkeMessageClass a = (res & 0x1f8) >> 3;
	if ( a == ARKE_ZEUS_SET_POINT && dlc == sizeof(ArkeZeusSetPoint) ) {
		if( rtr && yaacl_txn_status(&Z.txSetPoint) != YAACL_TXN_PENDING ) {
			ArkeZeusSetPoint sp;
			sp.Humidity = ClimateControllerGetHumidity();
			sp.Temperature = ClimateControllerGetTemperature();
			sp.Wind = ClimateControllerGetWind();
			ArkeSendZeusSetPoint(&Z.txSetPoint,false,&sp);
			return;
		}

		if (!rtr && dlc == sizeof(ArkeZeusSetPoint) )  {
			ClimateControlSetTarget(&Z.inBuffer.sp);

		}
	}


}


int main() {

	InitLEDs();
	InitSensors();
	InitHeaters();
	InitFanControl();
	InitClimateControl();
	InitZeus();

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

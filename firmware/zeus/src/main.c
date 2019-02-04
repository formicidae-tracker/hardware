#include "config.h"

#include "LEDs.h"
#include "Sensors.h"
#include "FanControl.h"
#include "Heaters.h"
#include "ClimateController.h"

#include <arke-avr.h>
#include <yaail.h>
#include <avr/interrupt.h>

implements_ArkeSoftwareReset()

typedef struct {
	yaacl_txn_t report,txSetPoint,status,txConfig,txControlPoint;
	ArkeZeusStatus statusData;
	ArkeZeusControlPoint cpData;
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
	yaacl_init_txn(&Z.report);
	yaacl_init_txn(&Z.txSetPoint);
	yaacl_init_txn(&Z.status);
	yaacl_init_txn(&Z.txConfig);
	yaacl_init_txn(&Z.txControlPoint);


	SetFan1Power(0);
	SetFan2Power(0);
	sei();
}


void ProcessIncoming() {
	uint8_t dlc;
	yaacl_idt_t res = ArkeProcess(&dlc);
	bool rtr = yaacl_idt_test_rtrbit(res);
	ArkeMessageClass a = (res & 0x1f8) >> 3;
	if ( a == ARKE_ZEUS_SET_POINT ) {
		if( rtr && yaacl_txn_status(&Z.txSetPoint) != YAACL_TXN_PENDING ) {
			ArkeZeusSetPoint sp;
			sp.Humidity = ClimateControllerGetHumidityTarget();
			sp.Temperature = ClimateControllerGetTemperatureTarget();
			sp.Wind = ClimateControllerGetWindTarget();
			ArkeSendZeusSetPoint(&Z.txSetPoint,false,&sp);
			return;
		}

		if (!rtr && dlc == sizeof(ArkeZeusSetPoint) )  {
			ClimateControllerSetTarget(&Z.inBuffer.sp);
			return;
		}
	}

	if ( a == ARKE_ZEUS_CONFIG ) {
		if  ( rtr && yaacl_txn_status(&Z.txConfig) != YAACL_TXN_PENDING ) {
			ArkeZeusConfig config;
			ClimateControllerGetConfig(&config);
			ArkeSendZeusConfig(&Z.txConfig,false,&config);
			return;
		}

		if (!rtr && dlc == sizeof(ArkeZeusConfig) ) {
			ClimateControllerConfigure(&Z.inBuffer.config);
			return;
		}
	}

	if ( a == ARKE_ZEUS_STATUS && rtr && yaacl_txn_status(&Z.status) != YAACL_TXN_PENDING ) {
		ArkeSendZeusStatus(&Z.status,false,&Z.statusData);
		return;
	}

	if ( a == ARKE_ZEUS_CONTROL_POINT && rtr && yaacl_txn_status(&Z.txControlPoint) != YAACL_TXN_PENDING ) {
		ArkeSendZeusControlPoint(&Z.txControlPoint,false,&Z.cpData);
		return;
	}

	if ( a == ARKE_ZEUS_REPORT && rtr && yaacl_txn_status(&Z.report) != YAACL_TXN_PENDING) {
		ArkeSendZeusReport(&Z.report,false,GetSensorData());
		return;
	}

}


int main() {

	InitLEDs();
	yaail_init(YAAIL_25);
	InitSensors();
	InitHeaters();
	InitFanControl();
	InitZeus();
	// needs Communication on
	InitClimateController();


	ArkeSystime_t lastCriticalStatus = ArkeGetSystime();

	while(true) {
		ProcessLEDs();
		ProcessIncoming();

 		bool hasNew =ProcessFanControl();
 		if (hasNew) {
 			Z.statusData.Fan[0] = GetFan1RPM();
 			Z.statusData.Fan[1] = GetFan2RPM();
 		}
		ArkeSystime_t now = ArkeGetSystime();
 		hasNew = ProcessSensors(now);
		ProcessHeater(now);
		if (hasNew) {
			LEDErrorToggle();
		}
		if ( hasNew && yaacl_txn_status(&Z.report) != YAACL_TXN_PENDING) {
			ArkeSendZeusReport(&Z.report,false,GetSensorData());
		}
		ClimateControllerProcess(hasNew,now);
		Z.statusData.Status = ClimateControllerStatus();
		if ( hasNew && (Z.statusData.Status & ARKE_ZEUS_ACTIVE) != 0) {
			Z.cpData.Humidity = ClimateControllerGetHumidityCommand();
			Z.cpData.Temperature = ClimateControllerGetTemperatureCommand();
#ifdef ZEUS_DEBUG_CONTROL
			if(yaacl_txn_status(&Z.txControlPoint) != YAACL_TXN_PENDING ) {
				ArkeSendZeusControlPoint(&Z.txControlPoint,false,&Z.cpData);
			}
#endif
		}

#define fan_has_no_error(fanRPM) ( (fanRPM & (ARKE_FAN_AGING_ALERT | ARKE_FAN_STALL_ALERT)) == 0x00)

		if ( (Z.statusData.Status & (ARKE_ZEUS_CLIMATE_UNCONTROLLED_WD|ARKE_ZEUS_TEMPERATURE_UNREACHABLE|ARKE_ZEUS_HUMIDITY_UNREACHABLE)) == 0
		     && fan_has_no_error(GetFan1RPM())
		     && fan_has_no_error(GetFan2RPM())) {
			continue;
		}

		if ( (now - lastCriticalStatus) >= 1000  && yaacl_txn_status(&Z.status) != YAACL_TXN_PENDING ) {
			ArkeSendZeusStatus(&Z.status,false,&Z.statusData);
			lastCriticalStatus = now;
		}

	}

	return 0;
}

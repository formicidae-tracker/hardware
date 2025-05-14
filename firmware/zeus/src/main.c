#include "arke.h"
#include "config.h"

#include "ClimateController.h"
#include "FanControl.h"
#include "Heaters.h"
#include "LEDs.h"
#include "Sensors.h"

#include <arke-avr.h>
#include <avr/interrupt.h>
#include <yaail.h>

implements_ArkeSoftwareReset()

    typedef struct {
	yaacl_txn_t    report, txSetPoint, status, txConfig, txControlPoint, deltaT;
	ArkeZeusStatus statusData;
	ArkeZeusControlPoint cpData;
	ArkeSystime_t        reportPeriod, lastReport;

	union {
		ArkeZeusSetPoint         sp;
		ArkeZeusConfig           config;
		ArkeZeusDeltaTemperature delta;
		ArkeZeusControlPoint     cp;
		uint8_t                  bytes[8];
	} inBuffer;

	uint8_t LEDStealthMode;
} Zeus_t;

Zeus_t Z;


void InitZeus() {
	InitArke(Z.inBuffer.bytes,8);
	Z.reportPeriod = 0;
	Z.lastReport = 0;
	Z.LEDStealthMode = 0;

	LEDReadyPulse();
	yaacl_init_txn(&Z.report);
	yaacl_init_txn(&Z.txSetPoint);
	yaacl_init_txn(&Z.status);
	yaacl_init_txn(&Z.txConfig);
	yaacl_init_txn(&Z.txControlPoint);
	yaacl_init_txn(&Z.deltaT);


	SetFan1Power(0);
	SetFan2Power(0);
	sei();
}

void ProcessIncoming() {
	uint8_t          dlc;
	yaacl_idt_t      res = ArkeProcess(&dlc);
	bool             rtr = yaacl_idt_test_rtrbit(res);
	ArkeMessageClass a   = (res & 0x1f8) >> 3;
	if (a == ARKE_ZEUS_SET_POINT) {
		if (rtr && yaacl_txn_status(&Z.txSetPoint) != YAACL_TXN_PENDING) {
			ArkeZeusSetPoint sp;
			sp.Humidity    = ClimateControllerGetHumidityTarget();
			sp.Temperature = ClimateControllerGetTemperatureTarget();
			sp.Wind        = ClimateControllerGetWindTarget();
			ArkeSendZeusSetPoint(&Z.txSetPoint, false, &sp);
			return;
		}

		if (!rtr && dlc == sizeof(ArkeZeusSetPoint)) {
			ClimateControllerSetTarget(&Z.inBuffer.sp);
			Z.LEDStealthMode = 1;
			LEDReadyOff();
			LEDErrorOff();
			return;
		}
	}

	if (a == ARKE_ZEUS_CONFIG) {
		if (rtr && yaacl_txn_status(&Z.txConfig) != YAACL_TXN_PENDING) {
			ArkeZeusConfig config;
			ClimateControllerGetConfig(&config);
			ArkeSendZeusConfig(&Z.txConfig, false, &config);
			return;
		}

		if (!rtr && dlc == sizeof(ArkeZeusConfig)) {
			ClimateControllerConfigure(&Z.inBuffer.config);
			return;
		}
	}

	if (a == ARKE_ZEUS_STATUS && rtr &&
	    yaacl_txn_status(&Z.status) != YAACL_TXN_PENDING) {
		ArkeSendZeusStatus(&Z.status, false, &Z.statusData);
		return;
	}

	if (a == ARKE_ZEUS_CONTROL_POINT) {
		if (rtr && yaacl_txn_status(&Z.txControlPoint) != YAACL_TXN_PENDING) {
			ArkeSendZeusControlPoint(&Z.txControlPoint, false, &Z.cpData);
			return;
		}

		if (!rtr && dlc == sizeof(ArkeZeusControlPoint)) {
			ClimateControllerSetRaw(&Z.inBuffer.cp);
			return;
		}
	}

	if (a == ARKE_ZEUS_REPORT && rtr &&
	    yaacl_txn_status(&Z.report) != YAACL_TXN_PENDING) {
		ArkeSendZeusReport(&Z.report, false, GetSensorData());
		return;
	}

	if (a == ARKE_ZEUS_DELTA_TEMPERATURE) {
		if (rtr && yaacl_txn_status(&Z.deltaT) != YAACL_TXN_PENDING) {
			ArkeSendZeusDeltaTemperature(
			    &Z.deltaT,
			    false,
			    SensorsGetDeltaTemperature()
			);
			return;
		}

		if (!rtr && dlc == sizeof(ArkeZeusDeltaTemperature)) {
			SensorsSetDeltaTemperature(&Z.inBuffer.delta);
		}
	}
}

int main() {

	InitLEDs();
	yaail_init(YAAIL_50);
	InitSensors();
	InitHeaters();

	FanRPMRange_e ranges[4] =
	    {RANGE_1000_RPM, RANGE_2000_RPM, RANGE_2000_RPM, RANGE_2000_RPM};
	uint8_t minValues[4] = {0x30, 0x37, 0x37, 0x37};
	InitFanControl(ranges, minValues);

	InitZeus();
	// needs Communication on
	InitClimateController();

	ArkeSystime_t lastCriticalStatus = ArkeGetSystime();

	while (true) {
		yaail_start_pending_txn();
		ProcessLEDs();
		ProcessIncoming();

		bool hasNew = ProcessFanControl();
		if (hasNew) {
			Z.statusData.Fan[0] = GetFan1RPM();
			Z.statusData.Fan[1] = GetFan2RPM();
			Z.statusData.Fan[2] = GetFan3RPM();
		}
		ArkeSystime_t now = ArkeGetSystime();
		hasNew            = ProcessSensors(now);

		ProcessHeaters(now);

		if (hasNew && Z.LEDStealthMode == 0) {
			LEDErrorToggle();
		}
		if (hasNew && yaacl_txn_status(&Z.report) != YAACL_TXN_PENDING) {
			ArkeSendZeusReport(&Z.report, false, GetSensorData());
		}
		ClimateControllerProcess(hasNew, now);
		Z.statusData.Status = ClimateControllerStatus();
		if (hasNew && (Z.statusData.Status & ARKE_ZEUS_ACTIVE) != 0) {
#ifdef ZEUS_DEBUG_CONTROL
			Z.cpData.Humidity    = ClimateControllerGetHumidityCommand();
			Z.cpData.Temperature = ClimateControllerGetTemperatureCommand();
			if (yaacl_txn_status(&Z.txControlPoint) != YAACL_TXN_PENDING) {
				ArkeSendZeusControlPoint(&Z.txControlPoint, false, &Z.cpData);
			}
#endif
		}

#define fan_has_no_error(fanRPM)                                               \
	((fanRPM & (ARKE_FAN_AGING_ALERT | ARKE_FAN_STALL_ALERT)) == 0x00)

		if ((Z.statusData.Status & (ARKE_ZEUS_CLIMATE_UNCONTROLLED_WD |
		                            ARKE_ZEUS_TEMPERATURE_UNREACHABLE |
		                            ARKE_ZEUS_HUMIDITY_UNREACHABLE)) == 0 &&
		    fan_has_no_error(GetFan1RPM()) && fan_has_no_error(GetFan2RPM()) &&
		    fan_has_no_error(GetFan3RPM())) {
			continue;
		}

		if ((now - lastCriticalStatus) >= 500 &&
		    yaacl_txn_status(&Z.status) != YAACL_TXN_PENDING) {
			ArkeSendZeusStatus(&Z.status, false, &Z.statusData);
			lastCriticalStatus = now;
		}
	}

	return 0;
}

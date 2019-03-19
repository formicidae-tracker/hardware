#include "ClimateController.h"

#include <avr/eeprom.h>
#include <avr/interrupt.h>
#include <stdbool.h>

#include <arke-avr.h>
#include <yaacl.h>

#include "Heaters.h"
#include "FanControl.h"
#include "LEDs.h"
#include "Sensors.h"
#include "PIDController.h"
#include "math.h"



#define WATCHDOG_MS 5000
#define DEFAULT_HUMIDITY 4915  //30%
#define DEFAULT_TEMPERATURE 6354 // 24°C


ArkePIDConfig EEMEM EEHumidity = {
	.ProportionalMult = 3,
	.DerivativeMult = 15,
	.IntegralMult = 0,
	.DividerPower = 6,
	.DividerPowerInt = 15,
};

ArkePIDConfig EEMEM EETemperature = {
	.ProportionalMult = 15,
	.DerivativeMult = 0,
	.IntegralMult = 0,
	.DividerPower = 6,
	.DividerPowerInt = 15,
};



struct ClimateControl_t {
	PIDController Humidity,Temperature;
	uint8_t       Wind;
	yaacl_txn_t   CelaenoCommand;
	ArkeSystime_t LastUpdate;
	ArkeSystime_t LastCommand;
	int16_t       TemperatureCommand,HumidityCommand;
	uint8_t       Status;
};

struct ClimateControl_t CC;


void InitClimateController() {
	ArkePIDConfig h,t;
	uint8_t sreg = SREG;
	cli();
	eeprom_read_block(&h,&EEHumidity,sizeof(ArkePIDConfig));
	eeprom_read_block(&t,&EETemperature,sizeof(ArkePIDConfig));
	SREG = sreg;
	InitPIDController(&CC.Humidity,1,0,255);
	PIDSetConfig(&CC.Humidity,&h);
	PIDSetTarget(&CC.Humidity,DEFAULT_HUMIDITY);
	InitPIDController(&CC.Temperature,1,0,510);
	PIDSetConfig(&CC.Temperature,&t);
	PIDSetTarget(&CC.Temperature,DEFAULT_TEMPERATURE);

	CC.Wind = 0;
	yaacl_init_txn(&(CC.CelaenoCommand));
	CC.LastUpdate = ArkeGetSystime();
	CC.Status = ARKE_ZEUS_IDLE;
}

void ClimateControllerSetTarget(const ArkeZeusSetPoint * sp) {
	PIDSetTarget(&CC.Humidity, sp->Humidity);
	PIDSetTarget(&CC.Temperature, sp->Temperature);
	CC.Wind = sp->Wind;
	CC.Status |= ARKE_ZEUS_ACTIVE;
}

void ClimateControllerConfigure(const ArkeZeusConfig * c) {
#define update_config(type) do {	  \
		PIDSetConfig(&CC.type,&(c->type)); \
		uint8_t sreg = SREG; \
		cli(); \
		eeprom_update_block(&(c->type),&EE ## type,sizeof(ArkePIDConfig)); \
		SREG = sreg; \
	}while(0)
	update_config(Humidity);
	update_config(Temperature);
#undef update_config
}


void ClimateControllerUpdateUnsafe(const ArkeZeusReport * r,ArkeSystime_t now) {
	ArkeSystime_t ellapsed = now - CC.LastUpdate;
	CC.LastUpdate = now;

	CC.HumidityCommand = PIDCompute(&CC.Humidity,r->Humidity,ellapsed);
	CC.TemperatureCommand = PIDCompute(&CC.Temperature,r->Temperature1,ellapsed);


	uint16_t heatPower;
	uint16_t ventPower;
	uint8_t windPower;
	ArkeCelaenoSetPoint sp;
	sp.Power = clamp(CC.HumidityCommand,0,255);

	if ( CC.TemperatureCommand > 0 ) {
		heatPower = min(510,CC.TemperatureCommand);
		windPower = max(0x40,CC.Wind);
		ventPower = 0;
	} else {
		heatPower = 0;
		windPower = CC.Wind;
		ventPower = clamp(-CC.TemperatureCommand,0,510) ;
	}

	HeaterSetPower1(heatPower/2);
	HeaterSetPower2(heatPower/2);
	SetFan1Power(windPower);
	SetFan2Power(clamp(ventPower,0,255));
	if (ventPower > 255) {
		SetFan3Power(clamp(ventPower-255,0,255));
	} else {
		SetFan3Power(0);
	}
	if ( yaacl_txn_status(&(CC.CelaenoCommand)) != YAACL_TXN_PENDING ) {
		ArkeSendCelaenoSetPoint(&(CC.CelaenoCommand),false,&sp);
	}

	if(PIDIntegralOverflow(&CC.Humidity) == true) {
		CC.Status |= ARKE_ZEUS_HUMIDITY_UNREACHABLE;
	} else {
		CC.Status &= ~(ARKE_ZEUS_HUMIDITY_UNREACHABLE);
	}

	if (PIDIntegralOverflow(&CC.Temperature) == true) {
		CC.Status |= ARKE_ZEUS_TEMPERATURE_UNREACHABLE;
	} else {
		CC.Status &= ~(ARKE_ZEUS_TEMPERATURE_UNREACHABLE);
	}

}

void ClimateControllerProcess(bool hasNewData,ArkeSystime_t now) {
	const ArkeZeusReport * r = GetSensorData();

	if ( hasNewData
	     && ( (CC.Status & ARKE_ZEUS_ACTIVE) != 0 ) ) {
		if ( r->Humidity == 0x3fff || r->Temperature1 == 0x3fff ) {
			ArkeReportError(0x0020);
		} else {
			ClimateControllerUpdateUnsafe(r,now);
			CC.Status &= ~ARKE_ZEUS_CLIMATE_UNCONTROLLED_WD;
			CC.LastCommand = now;
			return;
		}
	}

	if ( (now - CC.LastUpdate ) >= WATCHDOG_MS ) {
		CC.Status |= ARKE_ZEUS_CLIMATE_UNCONTROLLED_WD;
		// we should not move in the dark or we will hit walls pretty badly.
		// If no data is available for too long, we just ensure Celaeno is not running
		// and that we stop heating as we do not want to cook things.
		if ( (now - CC.LastCommand) >= 1000 && yaacl_txn_status(&(CC.CelaenoCommand))  != YAACL_TXN_PENDING ) {
			HeaterSetPower1(0);
			HeaterSetPower2(0);
			SetFan2Power(255);
			SetFan3Power(255);
			ArkeCelaenoSetPoint sp;
			sp.Power = 0;
			ArkeSendCelaenoSetPoint(&(CC.CelaenoCommand),false,&sp);
			CC.LastCommand = now;
		}
	}
}


uint8_t ClimateControllerStatus() {
	return CC.Status;
}

int16_t ClimateControllerGetHumidityCommand() {
	return CC.HumidityCommand;
}

int16_t ClimateControllerGetTemperatureCommand() {
	return CC.TemperatureCommand;
}


uint8_t ClimateControllerGetWindTarget() {
	return CC.Wind;
}
uint16_t ClimateControllerGetHumidityTarget() {
	return PIDGetTarget(&CC.Humidity);
}
uint16_t ClimateControllerGetTemperatureTarget() {
	return PIDGetTarget(&CC.Temperature);
}

void ClimateControllerGetConfig(ArkeZeusConfig * config) {
	config->Humidity = *PIDGetConfig(&CC.Humidity);
	config->Temperature = *PIDGetConfig(&CC.Temperature);
}

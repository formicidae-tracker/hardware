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

#define abs(a) ( ((a) < 0) ? -(a) : (a) )
#define max(a,b) ((a) > (b) ? (a) : (b) )
#define min(a,b) ((a) < (b) ? (a) : (b) )
#define clamp(value,low,high) max(min(value,high),low)



#define WATCHDOG_MS 5000
#define DEFAULT_HUMIDITY 4915  //30%
#define DEFAULT_TEMPERATURE 6354 // 24°C


ArkePDConfig EEMEM EEHumidity = {
	.ProportionalMult = 3,
	.DerivativeMult = 15,
	.IntegralMult = 0,
	.DividerPower = 6,
	.DividerPowerInt = 15,
};

ArkePDConfig EEMEM EETemperature = {
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
	int16_t       TemperatureCommand,HumidityCommand;
	uint8_t       Status;
};

struct ClimateControl_t CC;


void InitClimateController() {
	ArkePDConfig h,t;
	uint8_t sreg = SREG;
	cli();
	eeprom_read_block(&h,&EEHumidity,sizeof(ArkePDConfig));
	eeprom_read_block(&t,&EETemperature,sizeof(ArkePDConfig));
	SREG = sreg;
	InitPIDController(&CC.Humidity);
	PIDSetConfig(&CC.Humidity,&h);
	PIDSetTarget(&CC.Humidity,DEFAULT_HUMIDITY);
	InitPIDController(&CC.Temperature);
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
		eeprom_update_block(&(c->type),&EE ## type,sizeof(ArkePDConfig)); \
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


	uint8_t heatPower;
	uint8_t ventPower;
	uint8_t windPower;
	ArkeCelaenoSetPoint sp;
	sp.Power = clamp(CC.HumidityCommand,0,255);

	if ( CC.TemperatureCommand > 0 ) {
		heatPower = min(255,CC.TemperatureCommand);
		windPower = max(0x40,CC.Wind);
		ventPower = 0;
	} else {
		if (sp.Power>0) {
			sp.Power = clamp(clamp(CC.HumidityCommand,0,255)+clamp(-CC.TemperatureCommand,0,255),0,255);
			ventPower = sp.Power/2;
		} else {
			ventPower = min(255,-CC.TemperatureCommand);
		}
		heatPower = 0;
		windPower = CC.Wind;
	}

	HeaterSetPower1(heatPower);
	HeaterSetPower2(heatPower);
	SetFan1Power(ventPower);
	SetFan2Power(windPower);
	if ( yaacl_txn_status(&(CC.CelaenoCommand)) != YAACL_TXN_PENDING ) {
		ArkeSendCelaenoSetPoint(&(CC.CelaenoCommand),false,&sp);
	}

}

void ClimateControllerProcess(bool hasNewData,ArkeSystime_t now) {
	const ArkeZeusReport * r = GetSensorData();
	if ( hasNewData
	     && ( (CC.Status & ARKE_ZEUS_ACTIVE) != 0 )
	     && r->Humidity != 0x3fff && r->Temperature1 != 0x3fff ) {
		ClimateControllerUpdateUnsafe(r,now);
		CC.Status &= ~ARKE_ZEUS_CLIMATE_UNCONTROLLED_WD;
		return;
	}


	if ( (now - CC.LastUpdate ) >= WATCHDOG_MS ) {
		CC.Status |= ARKE_ZEUS_CLIMATE_UNCONTROLLED_WD;
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

#include "ClimateController.h"

#include <avr/eeprom.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include <string.h>

#include <arke-avr.h>
#include <yaacl.h>

#include "Heaters.h"
#include "FanControl.h"
#include "LEDs.h"
#include "Sensors.h"

#define abs(a) ( ((a) < 0) ? -(a) : (a) )
#define max(a,b) ((a) > (b) ? (a) : (b) )
#define min(a,b) ((a) < (b) ? (a) : (b) )
#define clamp(value,low,high) max(min(value,high),low)



#define WATCHDOG_MS 5000
#define DEFAULT_HUMIDITY 4915  //30%
#define DEFAULT_TEMPERATURE 6354 // 24°C
#define UNSET_DERROR_VALUE 0xffffffff

struct PDController_t {
	ArkePDConfig config;
	int32_t lastError;
	int32_t integralError;
	uint16_t target;
	uint16_t lastMeasure[4];
	uint8_t idx;
};

typedef struct PDController_t PDController;


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


void InitPDController(PDController * c,uint16_t target,ArkePDConfig * config) {
	memcpy(&(c->config),config,sizeof(ArkePDConfig));
	c->target = target;
	c->lastError = UNSET_DERROR_VALUE;
	c->idx = 0;
	c->integralError = 0;
}

int16_t PDControllerCompute(PDController * c, uint16_t current , ArkeSystime_t ellapsed) {
	if (c->lastError == UNSET_DERROR_VALUE) {
		for ( uint8_t i = 0; i < 4; ++i ) {
			c->lastMeasure[i] = current;
		}
	}
	c->lastMeasure[c->idx]  = current;
	c->idx = (c->idx + 1) & 0x03;
	int32_t currentMean = 0;
	for ( uint8_t i = 0; i < 4; ++i ) {
		currentMean += c->lastMeasure[i];
	}
	currentMean /= 4;

	int32_t error = (int32_t)(c->target) - currentMean;
	int32_t derror;
	int32_t toAdd = error * ellapsed;
	toAdd /= 1000;
	if ( (c->integralError > 0) && (toAdd > (INT32_MAX - c->integralError) ) ) {
		c->integralError = INT32_MAX;
	} else if ( (c->integralError < 0) && (toAdd < (INT32_MIN - c->integralError) ) ) {
		c->integralError = INT32_MIN;
	} else {
		c->integralError += toAdd;
	}

	if (c->lastError != UNSET_DERROR_VALUE) {
		 derror = (error - c->lastError);
	} else {
		derror = 0;
	}

	derror *= 1000;
	derror /= ellapsed;

	int32_t res = (c->config.ProportionalMult * error) >> c->config.DividerPower;
	res	+= (c->config.DerivativeMult * derror) >> c->config.DividerPower;
	res += (c->config.IntegralMult * c->integralError ) >> (c->config.DividerPowerInt);

	c->lastError = error;
	return res;
}



struct ClimateControl_t {
	PDController  Humidity,Temperature;
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
	InitPDController(&CC.Humidity,DEFAULT_HUMIDITY,&h);
	InitPDController(&CC.Temperature,DEFAULT_TEMPERATURE,&t);

	CC.Wind = 0;
	yaacl_init_txn(&(CC.CelaenoCommand));
	CC.LastUpdate = ArkeGetSystime();
	CC.Status = ARKE_ZEUS_IDLE;
}

void ClimateControllerSetTarget(const ArkeZeusSetPoint * sp) {
	CC.Humidity.target = sp->Humidity;
	CC.Temperature.target = sp->Temperature;
	CC.Wind = sp->Wind;
	CC.Status |= ARKE_ZEUS_ACTIVE;
}

void ClimateControllerConfigure(const ArkeZeusConfig * c) {
#define update_config(type) do { \
		memcpy(&(CC.type.config),&(c->type),sizeof(ArkePDConfig)); \
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

	CC.HumidityCommand = PDControllerCompute(&CC.Humidity,r->Humidity,ellapsed);
	CC.TemperatureCommand = PDControllerCompute(&CC.Temperature,r->Temperature1,ellapsed);


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
	return CC.Humidity.target;
}
uint16_t ClimateControllerGetTemperatureTarget() {
	return CC.Temperature.target;
}

void ClimateControllerGetConfig(ArkeZeusConfig * config) {
	config->Humidity = CC.Humidity.config;
	config->Temperature = CC.Temperature.config;
}

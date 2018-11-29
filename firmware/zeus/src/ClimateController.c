#include "ClimateController.h"

#include <arke-avr/systime.h>

#include <avr/eeprom.h>


#include <string.h>

#include "Heaters.h"
#include "FanControl.h"
#include "LEDs.h"

#define abs(a) ( ((a) < 0) ? -(a) : (a) )
#define max(a,b) ((a) > (b) ? (a) : (b) )
#define min(a,b) ((a) < (b) ? (a) : (b) )
#define clamp(value,low,high) max(min(value,high),low)

struct PDController_t {
	ArkePDConfig config;
	int32_t lastError;
	uint16_t target;
};

typedef struct PDController_t PDController;


ArkePDConfig EEMEM EEHumidity = {
	.DeadRegion = 100,
	.ProportionalMult = 100,
	.DerivativeMult = 20,
	.ProportionalDivPower2 = 6,
	.DerivativeDivPower2 = 6,
};

ArkePDConfig EEMEM EETemperature = {
	.DeadRegion = 100,
	.ProportionalMult = 100,
	.DerivativeMult = 20,
	.ProportionalDivPower2 = 6,
	.DerivativeDivPower2 = 6,
};



void InitPDController(PDController * c,uint16_t target,ArkePDConfig * config) {
	memcpy(&(c->config),config,sizeof(ArkePDConfig));
	c->target = target;
	c->lastError = 0;
}

int16_t PDControllerCompute(PDController * c, uint16_t current , ArkeSystime_t ellapsed) {
	LEDErrorOn();
	int32_t error = c->target - current;
	int32_t derror = (error - c->lastError);

	c->lastError = error;

	if ( abs(error) < c->config.DeadRegion ) {
		LEDErrorOff();
		return 0;
	}

	int32_t res = (c->config.ProportionalMult * error) >> c->config.ProportionalDivPower2;
	res	+= ((c->config.DerivativeMult * derror) / ellapsed ) >> c->config.DerivativeDivPower2;

	LEDErrorOff();
	return res;
}



struct ClimateControl_t {
	PDController Humidity,Temperature;
	ArkeSystime_t lastUpdate;
};

struct ClimateControl_t CC;


void InitClimateControl() {
	ArkePDConfig h,t;
	eeprom_read_block(&h,&EEHumidity,sizeof(ArkePDConfig));
	eeprom_read_block(&t,&EETemperature,sizeof(ArkePDConfig));

	InitPDController(&CC.Humidity,4914,&h);
	InitPDController(&CC.Temperature,4914,&t);

	CC.lastUpdate = ArkeGetSystime();
}

void ClimateControlSetTarget(const ArkeZeusSetPoint * sp) {
	CC.Humidity.target = sp->Humidity;
	CC.Temperature.target = sp->Temperature;
}

void ClimateControlConfigure(const ArkeZeusConfig * c) {
#define update_config(type) do { \
		memcpy(&(CC.type.config),&(c->type),sizeof(ArkePDConfig)); \
		eeprom_update_block(&(c->type),&EE ## type,sizeof(ArkePDConfig)); \
	}while(0)
	update_config(Humidity);
	update_config(Temperature);
#undef update_config
}

void ClimateControlUpdate(const ArkeZeusReport * r) {
	ArkeSystime_t now = ArkeGetSystime();
	ArkeSystime_t ellapsed = now - CC.lastUpdate;

	uint8_t humidityCommand = clamp(PDControllerCompute(&CC.Humidity,r->Humidity,ellapsed),0,255);

	//TODO: send real value
	SetFan3Power(humidityCommand);


	uint16_t tempCommand = PDControllerCompute(&CC.Temperature,r->Temperature1,ellapsed);

	uint8_t heatPower;
	uint8_t ventPower;

	if ( tempCommand > 0 ) {
		heatPower = min(255,tempCommand);
		ventPower = 0;
	} else {
		ventPower = min(255,-tempCommand);
		heatPower = 0;
	}

	HeaterSetPower1(heatPower);
	HeaterSetPower2(heatPower);
	if (heatPower > 0) {
		SetFan2Power(77); // 30% power
	} else {
		SetFan2Power(0);
	}

	SetFan1Power(ventPower);

}

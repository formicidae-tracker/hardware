#include "ClimateController.h"

#include <avr/eeprom.h>
#include <stdbool.h>
#include <string.h>

#include <arke-avr.h>
#include <yaacl.h>

#include "Heaters.h"
#include "FanControl.h"
#include "LEDs.h"

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
	c->lastError = UNSET_DERROR_VALUE;
}

int16_t PDControllerCompute(PDController * c, uint16_t current , ArkeSystime_t ellapsed) {
	LEDErrorOn();
	int32_t error = c->target - current;
	int32_t derror;
	if (c->lastError != UNSET_DERROR_VALUE) {
		 derror = (error - c->lastError);
	} else {
		derror = 0;
	}

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
	PDController  Humidity,Temperature;
	uint8_t       Wind;
	yaacl_txn_t   CelaenoCommand,ZeusReport;
	bool          Active;
	ArkeSystime_t LastUpdate;
};

struct ClimateControl_t CC;


void InitClimateControl() {
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
	CC.Active = false;
	CC.LastUpdate = ArkeGetSystime();
}

void ClimateControlSetTarget(const ArkeZeusSetPoint * sp) {
	CC.Humidity.target = sp->Humidity;
	CC.Temperature.target = sp->Temperature;
	CC.Wind = sp->Wind;
	CC.Active = false;
}

void ClimateControlConfigure(const ArkeZeusConfig * c) {
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



void ClimateControlUpdateUnsafe(const ArkeZeusReport * r,ArkeSystime_t now) {
	ArkeSystime_t ellapsed = now - CC.LastUpdate;
	CC.LastUpdate = now;


	ArkeCelaenoSetPoint sp;
	sp.Power = clamp(PDControllerCompute(&CC.Humidity,r->Humidity,ellapsed),0,255);

	if ( yaacl_txn_status(&(CC.CelaenoCommand)) != YAACL_TXN_PENDING ) {
		ArkeSendCelaenoSetPoint(&(CC.CelaenoCommand),false,&sp);
	}

	uint16_t tempCommand = PDControllerCompute(&CC.Temperature,r->Temperature1,ellapsed);

	uint8_t heatPower;
	uint8_t ventPower;
	uint8_t windPower;
	if ( tempCommand > 0 ) {
		heatPower = min(255,tempCommand);
		windPower = max(0x40,CC.Wind);
		ventPower = 0;
	} else {
		ventPower = min(255,-tempCommand);
		heatPower = 0;
		windPower = CC.Wind;
	}

	HeaterSetPower1(heatPower);
	HeaterSetPower2(heatPower);
	SetFan1Power(ventPower);
	SetFan2Power(windPower);
}



void ClimateControlUpdate(const ArkeZeusReport * r ) {
	ArkeSystime_t now = ArkeGetSystime();
	if ( CC.Active && r->Humidity != 0x3fff && r->Temperature1 != 0x3fff ) {
		ClimateControlUpdateUnsafe(r,now);
		return;
	}


	if ( (now - CC.LastUpdate ) < WATCHDOG_MS ) {
		return;
	}
	CC.LastUpdate = now;

	//TODO send alert message because climate is uncontrolled

}

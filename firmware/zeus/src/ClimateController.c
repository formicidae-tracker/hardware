#include "ClimateController.h"

#include <avr/eeprom.h>
#include <avr/interrupt.h>
#include <stdbool.h>

#include <arke-avr.h>
#include <yaacl.h>

#include "FanControl.h"
#include "Heaters.h"
#include "LEDs.h"
#include "PIDController.h"
#include "Sensors.h"
#include "arke-avr/systime.h"
#include "arke.h"
#include "math.h"
#include "zeus/src/FanControl.h"
#include "zeus/src/LEDs.h"

#define WATCHDOG_MS         5000
#define SENSOR_WATCHDOG_MS  3000
#define DEFAULT_HUMIDITY    4915 // 30%
#define DEFAULT_TEMPERATURE 6354 // 24°C
#define HUMIDITY_UNCONTROLLED_MIN_TIME                                         \
	60000 // 1m !!! Should not be more than 2^16-1 = 65535

ArkePIDConfig EEMEM EEHumidity = {
    .ProportionalMult = 10,
    .DerivativeMult   = 20,
    .IntegralMult     = 10,
    .DividerPower     = 5,
    .DividerPowerInt  = 15,
};

ArkePIDConfig EEMEM EETemperature = {
    .ProportionalMult = 100,
    .DerivativeMult   = 10,
    .IntegralMult     = 100,
    .DividerPower     = 5,
    .DividerPowerInt  = 15,
};

typedef enum ControlState {
	CC_NONE,
	CC_PID,
	CC_RAW,
} ControlState_e;

struct ClimateControl_t {
	PIDController  Humidity, Temperature;
	uint8_t        Wind;
	yaacl_txn_t    CelaenoCommand;
	ArkeSystime_t  LastSensorUpdate;
	ArkeSystime_t  LastCommand;
	ArkeSystime_t  LastHumidityControlled;
	int16_t        TemperatureCommand, HumidityCommand;
	uint8_t        Status;
	bool           SensorResetGuard;
	ControlState_e State;
};

struct ClimateControl_t CC;

void InitClimateController() {
	ArkePIDConfig h, t;
	uint8_t       sreg = SREG;
	cli();
	eeprom_read_block(&h, &EEHumidity, sizeof(ArkePIDConfig));
	eeprom_read_block(&t, &EETemperature, sizeof(ArkePIDConfig));
	SREG = sreg;
	InitPIDController(&CC.Humidity, 255);
	PIDSetConfig(&CC.Humidity, &h);
	PIDSetTarget(&CC.Humidity, DEFAULT_HUMIDITY);
	InitPIDController(&CC.Temperature, 511);
	PIDSetConfig(&CC.Temperature, &t);
	PIDSetTarget(&CC.Temperature, DEFAULT_TEMPERATURE);

	CC.Wind = 0;
	yaacl_init_txn(&(CC.CelaenoCommand));
	CC.LastSensorUpdate = ArkeGetSystime();
	CC.Status           = ARKE_ZEUS_IDLE;
	CC.SensorResetGuard = false;
	CC.State            = CC_NONE;
}

void ClimateControllerSetTarget(const ArkeZeusSetPoint *sp) {
	PIDSetTarget(&CC.Humidity, sp->Humidity);
	PIDSetTarget(&CC.Temperature, sp->Temperature);
	CC.Wind = sp->Wind;
	if (CC.State == CC_PID) {
		return;
	}
	ArkeSystime_t now = ArkeGetSystime();
	CC.Status &=
	    ~(ARKE_ZEUS_CLIMATE_UNCONTROLLED_WD |
	      ARKE_ZEUS_TEMPERATURE_UNREACHABLE | ARKE_ZEUS_HUMIDITY_UNREACHABLE);
	CC.LastHumidityControlled = now;
	CC.LastCommand            = now;
	CC.Status |= ARKE_ZEUS_ACTIVE;
	CC.State = CC_PID;
}

void climateControllerSendCommands(ArkeSystime_t now) {
	uint16_t            heatPower;
	uint16_t            ventPower;
	uint8_t             windPower;
	ArkeCelaenoSetPoint sp;
	sp.Power = clamp(CC.HumidityCommand, 0, 255);

	if (CC.TemperatureCommand > 0) {
		heatPower = min(510, CC.TemperatureCommand);
		windPower = max(0x40, CC.Wind);
		ventPower = 0;
	} else {
		heatPower = 0;
		windPower = CC.Wind;
		ventPower = clamp(-CC.TemperatureCommand, 0, 510);
	}

	HeatersSetPower(heatPower / 2);

	SetFan1Power(windPower);
	SetFan2Power(clamp(ventPower, 0, 255));
	if (ventPower > 255) {
		SetFan3Power(clamp(ventPower - 255, 0, 255));
	} else {
		SetFan3Power(0);
	}
	if (yaacl_txn_status(&(CC.CelaenoCommand)) != YAACL_TXN_PENDING) {
		ArkeSendCelaenoSetPoint(&(CC.CelaenoCommand), false, &sp);
	}

	CC.LastCommand = now;
}

void ClimateControllerSetRaw(const ArkeZeusControlPoint *cp) {
	if (CC.State == CC_PID) {
		// we should not be under PID control.
		return;
	}
	ArkeSystime_t now = ArkeGetSystime();

	if (cp->Humidity < -511 || cp->Temperature < -511 || cp->Humidity > 511 ||
	    cp->Temperature > 511) {
		// we should stop
		if (CC.State == CC_NONE) {
			return;
		}

		CC.TemperatureCommand = 0;
		CC.HumidityCommand    = 0;
		CC.Wind               = 0;

		climateControllerSendCommands(now);

		CC.Status = ARKE_ZEUS_IDLE;
		CC.State  = CC_NONE;
		return;
	}

	CC.TemperatureCommand = cp->Temperature;
	CC.HumidityCommand    = cp->Humidity / 2;
	CC.Wind               = 255;

	CC.Status = ARKE_ZEUS_ACTIVE;
	CC.State  = CC_RAW;

	climateControllerSendCommands(now);
}

void ClimateControllerConfigure(const ArkeZeusConfig *c) {
#define update_config(type)                                                    \
	do {                                                                       \
		PIDSetConfig(&CC.type, &(c->type));                                    \
		uint8_t sreg = SREG;                                                   \
		cli();                                                                 \
		eeprom_update_block(&(c->type), &EE##type, sizeof(ArkePIDConfig));     \
		SREG = sreg;                                                           \
	} while (0)
	update_config(Humidity);
	update_config(Temperature);
#undef update_config
}

void climateControllerPIDUpdateUnsafe(
    const ArkeZeusReport *r, ArkeSystime_t now
) {
	ArkeSystime_t ellapsed = now - CC.LastSensorUpdate;

	CC.HumidityCommand = PIDCompute(&CC.Humidity, r->Humidity, ellapsed);
	CC.TemperatureCommand =
	    PIDCompute(&CC.Temperature, r->Temperature1, ellapsed);

	climateControllerSendCommands(now);

	if (PIDIntegralOverflow(&CC.Humidity) == true) {
		if ((now - CC.LastHumidityControlled) >=
		    HUMIDITY_UNCONTROLLED_MIN_TIME) {
			CC.Status |= ARKE_ZEUS_HUMIDITY_UNREACHABLE;
		}
	} else {
		CC.Status &= ~(ARKE_ZEUS_HUMIDITY_UNREACHABLE);
		CC.LastHumidityControlled = now;
	}

	if (PIDIntegralOverflow(&CC.Temperature) == true) {
		CC.Status |= ARKE_ZEUS_TEMPERATURE_UNREACHABLE;
	} else {
		CC.Status &= ~(ARKE_ZEUS_TEMPERATURE_UNREACHABLE);
	}
}

void climateControllerRawUpdateUnsafe(ArkeSystime_t now) {}

void ClimateControllerProcess(bool hasNewData, ArkeSystime_t now) {
	const ArkeZeusReport *r = GetSensorData();

	if (hasNewData == true) {
		CC.LastSensorUpdate = now;
	}

	if (hasNewData && ((CC.Status & ARKE_ZEUS_ACTIVE) != 0)) {
		if (r->Humidity == 0x3fff || r->Temperature1 == 0x3fff) {
			ArkeReportError(0x0020);
		} else {
			switch (CC.State) {
			case CC_PID:
				climateControllerPIDUpdateUnsafe(r, now);
				break;
			case CC_RAW:
				climateControllerRawUpdateUnsafe(now);
				break;
			case CC_NONE:
				break;
			}

			CC.Status &= ~ARKE_ZEUS_CLIMATE_UNCONTROLLED_WD;
			CC.SensorResetGuard = false;
			return;
		}
	}

	if ((now - CC.LastSensorUpdate) >= SENSOR_WATCHDOG_MS &&
	    CC.SensorResetGuard == false) {
		ArkeReportError(0x0021);
		SensorsReset();
		CC.SensorResetGuard = true;
	}

	if ((now - CC.LastCommand) >= WATCHDOG_MS) {
		static ArkeSystime_t lastStatusUpdate;
		if ((CC.Status & ARKE_ZEUS_CLIMATE_UNCONTROLLED_WD) == 0x00) {
			lastStatusUpdate = now - 1000;
		}

		CC.Status |= ARKE_ZEUS_CLIMATE_UNCONTROLLED_WD;

		// we should not move in the dark or we will hit walls pretty badly.
		// If no data is available for too long, we just ensure Celaeno is not
		// running and that we stop heating as we do not want to cook things.
		if ((now - lastStatusUpdate) >= 1000 &&
		    yaacl_txn_status(&(CC.CelaenoCommand)) != YAACL_TXN_PENDING) {
			HeatersSetPower(0);
			SetFan2Power(255);
			SetFan3Power(255);
			ArkeCelaenoSetPoint sp;
			sp.Power = 0;
			ArkeSendCelaenoSetPoint(&(CC.CelaenoCommand), false, &sp);
			lastStatusUpdate = now;
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

void ClimateControllerGetConfig(ArkeZeusConfig *config) {
	config->Humidity    = *PIDGetConfig(&CC.Humidity);
	config->Temperature = *PIDGetConfig(&CC.Temperature);
}

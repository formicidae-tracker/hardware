#pragma once

#include "inttypes.h"
#include "stdbool.h"
#include <arke-avr/systime.h>

#include <arke.h>


struct PIDController_t {
	ArkePIDConfig config;
	int32_t lastError;
	int32_t integralError;
	uint16_t target;
	uint16_t lastMeasure[4];
	uint8_t idx;
	int32_t integralOverflowMax;
	int32_t integralOverflowMin;
	int8_t negativeMultiplier;
	int8_t negativeDividerPower2;
	int32_t maxCommand;
};


struct PIDController_t;
typedef struct PIDController_t PIDController;

void InitPIDController(PIDController * c,uint8_t negativeMult,uint8_t negativeDivPower2,int16_t maxCommand);
void PIDSetTarget(PIDController *c, uint16_t target);
uint16_t PIDGetTarget(PIDController *c);
void PIDSetConfig(PIDController *c, const ArkePIDConfig *config);
const ArkePIDConfig* PIDGetConfig(PIDController *c);
int16_t PIDCompute(PIDController *c, uint16_t current, ArkeSystime_t ellapsed);
bool PIDIntegralOverflow(PIDController * c);

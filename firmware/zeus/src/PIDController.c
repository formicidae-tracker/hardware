#include "PIDController.h"

#define UNSET_DERROR_VALUE 0xffffffff

#include "string.h"


void InitPIDController(PIDController * c) {
	c->lastError = UNSET_DERROR_VALUE;
	c->idx = 0;
	c->integralError = 0;
}

void PIDSetTarget(PIDController *c,uint16_t target) {
	c->target = target;
}

void PIDSetConfig(PIDController *c,const ArkePIDConfig * config) {
	memcpy(&(c->config),config,sizeof(ArkePIDConfig));
	if (c->config.IntegralMult == 0) {
		c->integralOverflowThreshold = INT32_MAX;
	} else {
		c->integralOverflowThreshold = (255 << c->config.DividerPowerInt) / c->config.IntegralMult;
		c->integralOverflowThreshold *= 105;
		c->integralOverflowThreshold /= 100;
	}
}

int16_t PIDCompute(PIDController * c, uint16_t current , ArkeSystime_t ellapsed) {
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
	if ( (c->integralError > 0) && (toAdd > (c->integralOverflowThreshold - c->integralError) ) ) {
		c->integralError = c->integralOverflowThreshold;
	} else if ( (c->integralError < 0) && (toAdd < (-c->integralOverflowThreshold - c->integralError) ) ) {
		c->integralError = -c->integralOverflowThreshold;
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


uint16_t PIDGetTarget(PIDController *c) {
	return c->target;
}

const ArkePIDConfig* PIDGetConfig(PIDController *c) {
	return &(c->config);
}

#define abs(a) ((a)< 0 ? -(a):(a))

bool PIDIntegralOverflow(PIDController *c) {
	if (c->config.IntegralMult == 0) {
		return false;
	}
	return abs(c->integralError) >= c->integralOverflowThreshold;
}

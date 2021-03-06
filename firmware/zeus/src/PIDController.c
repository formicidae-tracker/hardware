#include "PIDController.h"

#define UNSET_DERROR_VALUE 0xffffffff

#include "string.h"
#include "math.h"


void InitPIDController(PIDController * c,uint8_t negativeMult,uint8_t negativeDivPower2,int16_t maxCommand) {
	c->lastError = UNSET_DERROR_VALUE;
	c->idx = 0;
	c->integralError = 0;
	c->integralOverflowMax = INT32_MAX;
	c->integralOverflowMin = INT32_MIN;
	c->negativeMultiplier = negativeMult;
	c->negativeDividerPower2 = negativeDivPower2;
	c->maxCommand = maxCommand;
	c->derrorMask = false;
	c->proportionalOverflow = false;
}

void PIDSetTarget(PIDController *c,uint16_t target) {
	c->target = target;
	c->derrorMask = true;
}

void PIDSetConfig(PIDController *c,const ArkePIDConfig * config) {
	memcpy(&(c->config),config,sizeof(ArkePIDConfig));
	if (c->config.IntegralMult == 0) {
		c->integralOverflowMax = INT32_MAX;
		c->integralOverflowMin = INT32_MIN;
	} else {
		c->integralOverflowMax = ((c->maxCommand) << c->config.DividerPowerInt) / c->config.IntegralMult;
		c->integralOverflowMax *= 120;
		c->integralOverflowMax /= 100;
		c->integralOverflowMin = (-c->integralOverflowMax) * ( (int32_t)1 << c->negativeDividerPower2);
		c->integralOverflowMin /= (int32_t)c->negativeMultiplier;
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

	int32_t res = (c->config.ProportionalMult * error) / ( (int32_t)1 << c->config.DividerPower);

	c->proportionalOverflow = (res >= c->maxCommand || res <= -c->maxCommand);

	if ( c->proportionalOverflow == true) {
		c->integralError = 0;
	} else {
		int32_t toAdd = error * ellapsed;
		toAdd /= 1000;
		if ( (c->integralError > 0) && (toAdd > (c->integralOverflowMax - c->integralError) ) ) {
			c->integralError = c->integralOverflowMax;
		} else if ( (c->integralError < 0) && (toAdd < (c->integralOverflowMin - c->integralError) ) ) {
			c->integralError = c->integralOverflowMin;
		} else {
			c->integralError += toAdd;
		}
	}

	if (c->lastError != UNSET_DERROR_VALUE || c->derrorMask == true) {
		derror = (error - c->lastError);
	} else {
		derror = 0;
	}

	c->derrorMask = false;

	derror *= 1000;
	derror /= ellapsed;


	res	+= (c->config.DerivativeMult * derror) / ( (int32_t)1 << c->config.DividerPower);
	res += (c->config.IntegralMult * c->integralError ) / ( (int32_t)1 << c->config.DividerPowerInt);
	if (res < 0 ) {
		res = (res * c->negativeMultiplier) / ( (int32_t)1 << c->negativeDividerPower2);
	}

	c->lastError = error;
	return res;
}


uint16_t PIDGetTarget(PIDController *c) {
	return c->target;
}

const ArkePIDConfig* PIDGetConfig(PIDController *c) {
	return &(c->config);
}

bool PIDIntegralOverflow(PIDController *c) {
	if (c->config.IntegralMult == 0) {
		return false;
	}
	if ( c->proportionalOverflow == true ) {
		return true;
	}
	return (c->integralError >= c->integralOverflowMax) || (c->integralError <= c->integralOverflowMin);
}

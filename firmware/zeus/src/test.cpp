#include <cstdint>
#include <cstring>

#include <limits>
#include <iostream>

extern "C" {

typedef uint16_t ArkeSystime_t;

struct ArkePDConfig_t {
	uint8_t DeadRegion;
	uint8_t ProportionalMult;
	uint8_t DerivativeMult;
	uint8_t ProportionalDivPower2:4;
	uint8_t DerivativeDivPower2:4;
} __attribute__((packed));

typedef struct ArkePDConfig_t ArkePDConfig;

struct PDController_t {
	ArkePDConfig config;
	int32_t lastError;
	uint16_t target;
};

#define abs(a) ( ((a) < 0) ? -(a) : (a) )

typedef struct PDController_t PDController;

void InitPDController(PDController * c,uint16_t target,ArkePDConfig * config) {
	memcpy(&(c->config),config,sizeof(ArkePDConfig));
	c->target = target;
	c->lastError = 0;
}

int16_t PDControllerCompute(PDController * c, uint16_t current , ArkeSystime_t ellapsed) {
	int32_t error = c->target - current;

	int32_t derror = (error - c->lastError);

	c->lastError = error;

	if ( abs(error) < c->config.DeadRegion ) {
		return 0;
	}

	int32_t res = (c->config.ProportionalMult * error) >> c->config.ProportionalDivPower2;
	res	+= ((c->config.DerivativeMult * derror) / ellapsed ) >> c->config.DerivativeDivPower2;


	return res;
}


}

float FromHumidity(uint16_t value) {
	if ( value >=0x3fff ) {
		return std::numeric_limits<float>::signaling_NaN();
	}
	return (float)value / 16382.0 * 100.0;
}

uint16_t ToHumidity(float value) {
	if ( value < 0 || value > 100.0 ) {
		return 0x3fff;
	}
	return value / 100.0 * 16382.0;
}

float FromTemperature(uint16_t value) {
	if ( value >=0x3fff ) {
		return std::numeric_limits<float>::signaling_NaN();
	}
	return (float)value / 16382.0 * 165.0 - 40.0;
}

uint16_t ToTemperature(float value) {
	if ( value < -40 || value > 125.0 ) {
		return 0x3fff;
	}
	return (value + 40.0) / 165.0 * 16382.0;
}

#include <vector>

int main() {

	PDController c;
	ArkePDConfig conf = {
		.DeadRegion = 100,
		.ProportionalMult = 100,
		.DerivativeMult = 100,
		.ProportionalDivPower2 =6,
		.DerivativeDivPower2=6,
	};

	InitPDController(&c,ToTemperature(25.0),&conf);

	std::vector<float> tempValues;
	tempValues.push_back(35.0);
	for(size_t i = 0; i < 100; ++i) {
		tempValues.push_back(0.1*(25.0-tempValues[i])+tempValues[i]);
	}

	std::vector<int16_t> commands;

	for( size_t i = 0 ; i < tempValues.size(); ++i ) {
		uint16_t current = ToTemperature(tempValues[i]);
		commands.push_back(PDControllerCompute(&c, current ,1000));
		std::cout << i << ": " << tempValues[i] << " " << current << " res:" << commands[i] << std::endl;
	}




}

#pragma once

#include <inttypes.h>

typedef enum {
	FAN_OK = 0,
	FAN_1_STALL = (1 << 0),
	FAN_2_STALL = (1 << 1),
} FanControlStatus_e;

void InitFanControl();

FanControlStatus_e ProcessFanControl();

void SetFan1Power(uint8_t value);
void SetFan2Power(uint8_t value);

uint16_t GetFan1RPM();
uint16_t GetFan2RPM();

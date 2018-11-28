#pragma once

#include <inttypes.h>
#include "config.h"


typedef enum {
	FAN_OK = 0,
	FAN_1_STALL = (1 << 0),
	FAN_2_STALL = (1 << 1),
#ifdef FAN_CONTROL_4FAN
	FAN_3_STALL = (1 << 2),
	FAN_4_STALL = (1 << 3),
#endif
} FanControlStatus_e;

void InitFanControl();

FanControlStatus_e ProcessFanControl();

void SetFan1Power(uint8_t value);
void SetFan2Power(uint8_t value);

uint16_t GetFan1RPM();
uint16_t GetFan2RPM();
uint8_t GetFan1Power();
uint8_t GetFan2Power();

#ifdef FAN_CONTROL_4FAN
void SetFan3Power(uint8_t value);
void SetFan4Power(uint8_t value);

uint16_t GetFan3RPM();
uint16_t GetFan4RPM();
uint8_t GetFan3Power();
uint8_t GetFan4Power();
#endif

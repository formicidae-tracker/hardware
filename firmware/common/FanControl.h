#pragma once

#include <inttypes.h>
#include <stdbool.h>
#include "config.h"
#include <arke.h>


typedef enum {
	RANGE_1000_RPM = 1,
	RANGE_2000_RPM= 2
} FanRPMRange_e;

#ifdef FAN_CONTROL_4FAN
void InitFanControl(FanRPMRange_e ranges[4],
                    uint8_t minValues[4]);
#else
void InitFanControl(FanRPMRange_e ranges[2],
                    uint8_t minValues[2]);
#endif

bool ProcessFanControl();

#define declare_fan_interface(i) \
	void SetFan ## i ## Power(uint8_t value); \
	uint16_t GetFan ## i ## RPM(); \
	uint8_t  GetFan ## i ## Power()

declare_fan_interface(1);
declare_fan_interface(2);

#ifdef FAN_CONTROL_4FAN
declare_fan_interface(3);
declare_fan_interface(4);
#endif

#undef declare_fan_interface

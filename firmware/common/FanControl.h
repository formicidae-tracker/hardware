#pragma once

#include <inttypes.h>
#include <stdbool.h>
#include "config.h"
#include <arke.h>


void InitFanControl();

bool ProcessFanControl();

void SetFan1Power(uint8_t value);
void SetFan2Power(uint8_t value);



ArkeFanStatus GetFan1RPM();
ArkeFanStatus GetFan2RPM();
uint8_t GetFan1Power();
uint8_t GetFan2Power();

#ifdef FAN_CONTROL_4FAN
void SetFan3Power(uint8_t value);
void SetFan4Power(uint8_t value);

ArkeFanStatus GetFan3RPM();
ArkeFanStatus GetFan4RPM();
uint8_t GetFan3Power();
uint8_t GetFan4Power();
#endif

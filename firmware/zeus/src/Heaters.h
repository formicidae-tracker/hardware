#pragma once

#include <inttypes.h>

#include <arke-avr/systime.h>

void InitHeaters();
void HeaterSetPower1(uint8_t power);
void HeaterSetPower2(uint8_t power);
void ProcessHeater(ArkeSystime_t now);

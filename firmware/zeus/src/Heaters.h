#pragma once

#include <inttypes.h>

#include <arke-avr/systime.h>

void InitHeaters();
void HeatersSetPower(uint8_t power);
void ProcessHeaters(ArkeSystime_t now);

#pragma once

#include <arke.h>



void InitClimateControl();

void ClimateControlSetTarget(const ArkeZeusSetPoint * sp);

void ClimateControlConfigure(const ArkeZeusConfig * c);

void ClimateControlUpdate(const ArkeZeusReport * r);

uint8_t ClimateControllerGetWind();
uint16_t ClimateControllerGetHumidity();
uint16_t ClimateControllerGetTemeprature();

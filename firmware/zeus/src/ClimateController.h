#pragma once

#include <arke.h>
#include <stdbool.h>

#include <arke-avr/systime.h>

void InitClimateController();

void ClimateControllerSetTarget(const ArkeZeusSetPoint *sp);

void ClimateControllerConfigure(const ArkeZeusConfig *c);

void ClimateControllerProcess(bool hasNewData, ArkeSystime_t now);

void ClimateControllerSetRaw(const ArkeZeusControlPoint *cp);

uint8_t ClimateControllerStatus();

#ifdef ZEUS_DEBUG_CONTROL
int16_t ClimateControllerGetHumidityCommand();
int16_t ClimateControllerGetTemperatureCommand();
#endif

uint8_t  ClimateControllerGetWindTarget();
uint16_t ClimateControllerGetHumidityTarget();
uint16_t ClimateControllerGetTemperatureTarget();
void     ClimateControllerGetConfig(ArkeZeusConfig *config);

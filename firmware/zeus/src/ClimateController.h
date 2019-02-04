#pragma once

#include <arke.h>
#include <stdbool.h>

#include <arke-avr/systime.h>

void InitClimateController();

void ClimateControllerSetTarget(const ArkeZeusSetPoint * sp);

void ClimateControllerConfigure(const ArkeZeusConfig * c);

void ClimateControllerProcess(bool hasNewData,ArkeSystime_t now);

uint8_t ClimateControllerStatus();

int16_t ClimateControllerGetHumidityCommand();
int16_t ClimateControllerGetTemperatureCommand();


uint8_t ClimateControllerGetWindTarget();
uint16_t ClimateControllerGetHumidityTarget();
uint16_t ClimateControllerGetTemperatureTarget();
void ClimateControllerGetConfig(ArkeZeusConfig *  config);

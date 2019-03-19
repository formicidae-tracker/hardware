#pragma once

#include <stdbool.h>

#include <arke.h>

#include <arke-avr/systime.h>


void InitSensors();

//true if there is new data received.
bool ProcessSensors(ArkeSystime_t now);

const ArkeZeusReport * GetSensorData();

void SensorsSetDeltaTemperature(const ArkeZeusDeltaTemperature * deltas);

const ArkeZeusDeltaTemperature * SensorsGetDeltaTemperature();


void SensorsReset();

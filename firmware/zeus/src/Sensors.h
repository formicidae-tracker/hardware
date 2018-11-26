#pragma once

#include <stdbool.h>

#include <arke.h>



void InitSensors();

//true if there is new data received.
bool ProcessSensors();

const ArkeZeusReport * GetSensorData();

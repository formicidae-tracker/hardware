#pragma once

#include <arke.h>



void InitClimateControl();

void ClimateControlSetTarget(const ArkeZeusSetPoint * sp);

void ClimateControlConfigure(const ArkeZeusConfig * c);

void ClimateControlUpdate(const ArkeZeusReport * r);

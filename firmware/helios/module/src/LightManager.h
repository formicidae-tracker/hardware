#pragma once

#include <inttypes.h>


typedef enum {
	VISIBLE = 0,
	UV = 1,
	IR = 2,
	NUM_CHANNELS
} channel_e;

void InitLightManager();

void LMActivateOutput();

void LMDeactivateOutput();

void LMSetBrightness(channel_e channel, uint8_t brightness);

uint16_t LMSystime();

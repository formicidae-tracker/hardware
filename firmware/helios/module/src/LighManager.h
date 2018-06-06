#pragma once

typedef enum {
	VISIBLE = 0,
	UV = 1
} channel_e;

void InitLightManager();

void LMActivateOutput();

void LMDeactivateOutput();

void LMSetBrightness(channel_e channel, uint8_t brightness);

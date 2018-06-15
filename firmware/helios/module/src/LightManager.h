#pragma once

#include <inttypes.h>

// Defines the available channels of the ligh manager
typedef enum {
	VISIBLE = 0,
	UV = 1,
	IR = 2,
	NUM_CHANNELS
} channel_e;


// Initializes the LightManager modules
void InitLightManager();

// Activates the outputs of the light manager
void LMActivateOutput();

// Deactivate the outputs of teh light manager
void LMDeactivateOutput();

// Sets the brightness of any channel
void LMSetBrightness(channel_e channel, uint8_t brightness);

// Returns a 610Hz counter ticks
uint16_t LMSystime();

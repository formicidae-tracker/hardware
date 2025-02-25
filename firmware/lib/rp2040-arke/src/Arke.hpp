#pragma once

#include <arke.h>

#include <cstddef>
#include <functional>

extern "C" {
struct can2040_msg;
}

struct ArkeEvent {
	ArkeMessageClass Class;
	bool             RTR = false;
	uint8_t          Size;
	const uint8_t   *Data;
};

struct ArkeConfig {
	unsigned int  PinRX, PinTX;
	unsigned int  PIO;
	ArkeNodeClass Class;
	ArkeNodeClass ClassMask;
	uint8_t       ID = 1;

	std::function<void(const ArkeEvent &)> Callback;
};

void ArkeInit(ArkeConfig &&config);

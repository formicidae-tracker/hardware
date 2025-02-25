#pragma once

#include <arke.h>

#include <cstddef>
#include <cstdint>
#include <functional>
#include <optional>

extern "C" {
struct can2040_msg;
}

struct ArkeEvent {
	ArkeMessageClass Class;
	bool             RTR = false;
	uint8_t          Size;
	const uint8_t   *Data;
};

typedef std::function<void(const ArkeEvent &)> ArkeCallback;
typedef std::
    tuple<uint8_t, uint8_t, std::optional<uint8_t>, std::optional<uint8_t>>
        ArkeNodeVersion;

struct ArkeConfig {
	unsigned int  PinRX, PinTX;
	unsigned int  PIO;
	ArkeNodeClass Class;
	ArkeNodeClass ClassMask;

	ArkeCallback Callback;

	std::optional<ArkeNodeVersion> NodeVersion;
};

void ArkeInit(ArkeConfig &&config);

uint8_t ArkeGetID();

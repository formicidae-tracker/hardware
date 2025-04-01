#pragma once

#include <type_traits>
extern "C" {
#include <arke.h>
}

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

	template <typename T, std::enable_if_t<sizeof(T) <= 8> * = nullptr>
	const T &as() const {
		return *reinterpret_cast<const T *>(Data);
	}
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

namespace details {
void arkeSend(ArkeMessageClass_e cls, const uint8_t *data, size_t size);
}

template <typename T>
inline void ArkeSend(ArkeMessageClass_e cls, const T &data) {
	static_assert(sizeof(T) <= 8, "Object data should be smaller than 8 byte");

	details::arkeSend(cls, (const uint8_t *)(&data), sizeof(T));
}

void ArkeScheduleStats(uint64_t period_us);

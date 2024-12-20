#pragma once

#include <array>
#include <cstdint>
#include <type_traits>

template <typename T, uint8_t N, std::enable_if_t<N >= 1> * = nullptr>
class RingBuffer {
public:
	template <typename U> inline void insert(U &&obj) {
		d_data[d_writePtr] = std::forward<U>(obj);
		increment(d_writePtr);
		++d_count;
	}

	template <typename... Args> inline void emplace(Args &&...args) {
		d_data[d_writePtr] = T{std::forward<Args>(args)...};
		increment(d_writePtr);
		++d_count;
	}

	inline void pop(T &obj) {
		obj = std::move(d_data[d_readPtr]);
		increment(d_readPtr);
		--d_count;
	}

	inline uint8_t size() const {
		return d_count;
	}

	inline bool empty() const {
		return d_count == 0;
	}

	inline bool full() const {
		return d_count == N;
	}

protected:
	inline void increment(uint8_t &pointer) {
		if (++pointer >= N) {
			pointer = 0;
		}
	}

	std::array<T, N> d_data;
	uint8_t          d_writePtr = 0;
	uint8_t          d_readPtr  = 0;
	uint8_t          d_count    = 0;
};

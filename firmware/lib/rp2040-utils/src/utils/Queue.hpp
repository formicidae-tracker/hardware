#pragma once

// this should be documented
extern "C" {
#include "pico/lock_core.h"
}

#include <array>
#include <cstdint>
#include <type_traits>

#include "RingBuffer.hpp"

template <typename T, size_t N, bool Threadsafe = true>
class Queue : protected RingBuffer<T, N> {
public:
	Queue() {
		lock_init(&d_core, next_striped_spin_lock_num());
	}

	inline size_t Size() const {
		auto   save = lock();
		size_t size = this->d_count;
		unlock(save);
		return size;
	}

	inline bool Empty() const {
		return Size() == 0;
	}

	inline bool Full() const {
		return Size() == N;
	}

	template <typename U> inline bool TryAdd(U &&obj) {
		return add(std::forward<U>(obj), false);
	}

	inline bool TryRemove(T &obj) {
		return remove(obj, false);
	}

	template <typename... Args> inline bool TryEmplace(Args &&...args) {
		return Queue::emplace(false, std::forward<Args>(args)...);
	}

	template <typename U> inline void AddBlocking(U &&obj) {
		this->add(std::forward<U>(obj), true);
	}

	inline void RemoveBlocking(T &obj) {
		this->remove(obj, true);
	}

	template <typename... Args> inline void EmplaceBlocking(Args &&...args) {
		Queue::emplace(true, std::forward<Args>(args)...);
	}

protected:
	lock_core_t d_core;

	inline uint32_t lock() const {
		return spin_lock_blocking(d_core.spin_lock);
	}

	inline void unlock(uint32_t saved) const {
		spin_unlock(d_core.spin_lock, saved);
	}

	inline void unlock_wait(uint32_t saved) const {
		lock_internal_spin_unlock_with_wait(&d_core, saved);
	}

	inline void unlock_notify(uint32_t saved) const {
		lock_internal_spin_unlock_with_notify(&d_core, saved);
	}

	template <typename U> inline bool add(U &&obj, bool block) {
		do {
			auto save = lock();
			if (this->d_count != N) {
				this->insert(std::forward<U>(obj));
				unlock_notify(save);
				return true;
			}
			if (block) {
				unlock_wait(save);
			} else {
				unlock(save);
				return false;
			}
		} while (true);
	}

	inline bool remove(T &obj, bool block) {
		do {
			auto save = lock();
			if (this->d_count > 0) {
				this->pop(obj);
				unlock_notify(save);
				return true;
			}
			if (block) {
				unlock_wait(save);
			} else {
				unlock(save);
				return false;
			}
		} while (true);
	}

	template <typename... Args>
	inline bool emplace(bool block, Args &&...args) {
		do {
			auto save = lock();
			if (this->d_count != N) {
				RingBuffer<T, N>::emplace(std::forward<Args>(args)...);
				unlock_notify(save);
				return true;
			}
			if (block) {
				unlock_wait(save);
			} else {
				unlock(save);
				return false;
			}
		} while (true);
	}

	inline void increment(uint16_t &value) {
		if (++value >= N) {
			value = 0;
		}
	}
};

template <typename T, size_t N>
class Queue<T, N, false> : protected RingBuffer<T, N> {
public:
	inline size_t Size() const {
		return this->d_count;
	}

	inline bool Empty() const {
		return this->d_count == 0;
	}

	inline bool Full() const {
		return this->d_count == N;
	}

	template <typename U> inline bool TryAdd(U &&obj) {
		if (this->d_count >= N) {
			return false;
		}
		this->insert(std::forward<U>(obj));
		return true;
	}

	inline bool TryRemove(T &obj) {
		if (this->d_count == 0) {
			return false;
		}
		this->pop(obj);
		return true;
	}

	template <typename... Args> bool TryEmplace(Args &&...args) {
		if (this->d_count >= N) {
			return false;
		}
		this->emplace(std::forward<Args>(args)...);
		return true;
	}
};

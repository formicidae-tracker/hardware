#pragma once

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <functional>
#include <map>
#include <memory>
#include <type_traits>
#include <vector>

#include <hardware/flash.h>
#include <pico/flash.h>

#include "utils/Log.hpp"

#ifndef PICO_NV_STORAGE_NB_SECTOR
#define PICO_NV_STORAGE_NB_SECTOR 1
#endif

template <typename T, uint16_t UUID, size_t PagesPerObject = 1>
class FlashStorage {
	struct Header;

public:
	using Type = std::remove_cv_t<std::remove_reference_t<T>>;

	static constexpr size_t MAX_OBJECT_SIZE =
	    PagesPerObject * FLASH_PAGE_SIZE - sizeof(Header);

	static constexpr size_t PAGES_PER_SECTOR =
	    FLASH_SECTOR_SIZE / FLASH_PAGE_SIZE;

	static_assert(
	    PagesPerObject > 0, "Invalid PagesPerObject: must be strictly positive"
	);
	static_assert(
	    PagesPerObject <= PAGES_PER_SECTOR,
	    "Invalid PagesPerObject: must fit in a single sector"
	);
	static_assert(
	    PAGES_PER_SECTOR % PagesPerObject == 0,
	    "Invalid PagesPerObject: must be a divisor of number of pages in a "
	    "sector"
	);

	static_assert(
	    sizeof(T) < MAX_OBJECT_SIZE,
	    "Maximal Allowed Object Size exceeded, try Increase "
	    "PagesPerObject"
	);

	inline static bool Load(T &obj) {
		std::tuple<bool, T &> params{false, obj};
		if (flash_safe_execute(load, &params, 500) != PICO_OK) {
			return false;
		}
		return std::get<0>(params);
	}

	inline static void Save(const T &obj) {
		if (flash_safe_execute(
		        save,
		        const_cast<void *>(reinterpret_cast<const void *>(&obj)),
		        500
		    ) != PICO_OK) {
			Errorf("[FlashStorage]: could not save to flash");
		}
	}

private:
	static constexpr uint XIP_OFFSET =
	    PICO_FLASH_SIZE_BYTES - PICO_NV_STORAGE_NB_SECTOR * FLASH_SECTOR_SIZE;
	static constexpr uint8_t MAGIC_WORD = 0xaa;

	struct Header {
		const uint8_t Begin = 0xaa;
		uint8_t       SizeInSector;
		uint16_t      Identifier;
	};

	inline static void
	forAllValidPages(const std::function<bool(const Header *)> &apply) {
		for (size_t i = 0; i < PAGES_PER_SECTOR * PICO_NV_STORAGE_NB_SECTOR;) {
			const Header *h = reinterpret_cast<const Header *>(
			    XIP_BASE + XIP_OFFSET + i * FLASH_PAGE_SIZE
			);
			if (h->Begin != MAGIC_WORD) {
				break;
			}
			if (apply(h) == false) {
				break;
			}
			i += h->SizeInSector;
		}
	}

	inline static void load(void *params_) {
		std::tuple<bool, T &> *params =
		    reinterpret_cast<std::tuple<bool, T &> *>(params_);

		Debugf("[FlashStorage]: Identifier=%08x", UUID);
		const Type *valid = nullptr;
		forAllValidPages([&valid](const Header *h) {
			if (h->Identifier == UUID && h->SizeInSector == PagesPerObject) {
				valid = reinterpret_cast<const Type *>(
				    reinterpret_cast<const uint8_t *>(h) + sizeof(Header)
				);
			}
			return true;
		});

		if (valid == nullptr) {
			std::get<0>(*params) = false;
			return;
		}
		std::get<1>(*params) = *valid;
		std::get<0>(*params) = true;
	}

	inline static size_t findFree() {
		for (size_t i = 0; i < PAGES_PER_SECTOR * PICO_NV_STORAGE_NB_SECTOR;) {
			const Header *h = reinterpret_cast<const Header *>(
			    XIP_BASE + XIP_OFFSET + i * FLASH_PAGE_SIZE
			);
			if (h->Begin == MAGIC_WORD) {
				i += h->SizeInSector;
				continue;
			}
			return i;
		}
		return PAGES_PER_SECTOR * PICO_NV_STORAGE_NB_SECTOR;
	}

	inline static void save(void *obj_) {
		const Type *obj = reinterpret_cast<const Type *>(obj_);
		if (isSame(*obj)) {
			Debugf("[FlashStorage]: not saving as it is unchanged");
			return;
		}

		size_t idx = findFree();

		if (idx == PAGES_PER_SECTOR * PICO_NV_STORAGE_NB_SECTOR) {
			erase();
		}

		uint8_t buffer[PagesPerObject * FLASH_PAGE_SIZE];
		Header  h = {
		     .SizeInSector = PagesPerObject,
		     .Identifier   = UUID,
        };
		memcpy(buffer, &h, sizeof(Header));
		memcpy(buffer + sizeof(Header), obj, sizeof(Type));
		memset(
		    buffer + sizeof(Header) + sizeof(Type),
		    0x00,
		    PagesPerObject * FLASH_PAGE_SIZE - sizeof(Header) - sizeof(Type)
		);

		flash_range_program(
		    XIP_OFFSET + idx * PagesPerObject * FLASH_PAGE_SIZE,
		    buffer,
		    PagesPerObject * FLASH_PAGE_SIZE
		);
		Infof("[FlashStorage]: saved at page %d", idx * PagesPerObject);
	}

	inline static void erase() {
		// We need to save all values that will not be affected
		std::map<uint16_t, std::vector<uint8_t>> saved;
		forAllValidPages([&](const Header *h) {
			if (h->Identifier == UUID) {
				return true;
			}
			saved[h->Identifier].resize(h->SizeInSector * FLASH_PAGE_SIZE);
			memcpy(
			    saved[h->Identifier].data(),
			    reinterpret_cast<const uint8_t *>(h),
			    h->SizeInSector * FLASH_PAGE_SIZE
			);
			return true;
		});
		flash_range_erase(
		    XIP_OFFSET,
		    PICO_NV_STORAGE_NB_SECTOR * FLASH_SECTOR_SIZE
		);
		size_t i = 0;
		for (const auto &[_, data] : saved) {
			flash_range_program(XIP_OFFSET + i, data.data(), data.size());
			i += data.size();
		}
	}

	inline static bool isSame(const T &obj) {
		Type saved;
		if (!Load(saved)) {
			return false;
		}
		return memcmp(&saved, &obj, sizeof(Type)) == 0;
	}
};

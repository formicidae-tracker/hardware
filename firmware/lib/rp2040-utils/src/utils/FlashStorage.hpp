#pragma once

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <functional>
#include <map>
#include <memory>
#include <type_traits>
#include <vector>

extern "C" {
#include <hardware/flash.h>
#include <pico/multicore.h>
#include <pico/time.h>
#include <stdio.h>
}

#include <utils/Defer.hpp>

#ifndef PICO_NV_STORAGE_NB_SECTOR
#define PICO_NV_STORAGE_NB_SECTOR 1
#endif

#ifdef NDEBUG
#define debugf(...)                                                            \
	do {                                                                       \
	} while (0)
#else
#define debugf(...)                                                            \
	do {                                                                       \
		printf(__VA_ARGS__);                                                   \
	} while (0)
#endif

namespace details {
void PrintMemory(const uint8_t *addr, size_t size);
void PrintFlashStorage();
} // namespace details

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
		// auto saved = save_and_disable_interrupts();
		multicore_lockout_start_blocking();
		defer {
			multicore_lockout_end_blocking();
			// restore_interrupts_from_disabled(saved);
		};
		return load(obj);
	}

	inline static bool Save(const T &obj) {
		multicore_lockout_start_blocking();
		defer {
			multicore_lockout_end_blocking();
		};
		return save(obj);
	}

private:
	static constexpr uint XIP_OFFSET =
	    PICO_FLASH_SIZE_BYTES - PICO_NV_STORAGE_NB_SECTOR * FLASH_SECTOR_SIZE;
	static constexpr uint8_t MAGIC_WORD = 0xaa;

	struct Header {
		const uint8_t Begin = 0xaa;
		uint8_t       SizeInPages;
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
			i += h->SizeInPages;
		}
	}

	inline static int pageIndex(int addr) {
		return (addr - XIP_BASE - XIP_OFFSET) / FLASH_PAGE_SIZE;
	}

	inline static bool load(T &obj) {
		debugf("[FlashStorage]: Identifier=%08x\n");
		const Type *valid = nullptr;
		forAllValidPages([&valid](const Header *h) {
			if (h->Identifier == UUID && h->SizeInPages == PagesPerObject) {
				valid = reinterpret_cast<const Type *>(
				    reinterpret_cast<const uint8_t *>(h) + sizeof(Header)
				);
			}
			return true;
		});

		if (valid == nullptr) {
			debugf("[FlashStorage]: No Pages found\n", UUID);
			return false;
		}
		debugf(
		    "[FlashStorage]: Pages %d matches\n",
		    pageIndex(int(valid) - sizeof(Header))
		);
		obj = *valid;
		return true;
	}

	inline static size_t findFree() {
		for (size_t i = 0; i < PAGES_PER_SECTOR * PICO_NV_STORAGE_NB_SECTOR;) {
			const Header *h = reinterpret_cast<const Header *>(
			    XIP_BASE + XIP_OFFSET + i * FLASH_PAGE_SIZE
			);
			if (h->Begin == MAGIC_WORD) {
				i += h->SizeInPages;
				continue;
			}
			debugf("[FlashStorage]: page %d is free\n", i);
			return i;
		}
		debugf("[FlashStorage]: no page is free\n");
		return PAGES_PER_SECTOR * PICO_NV_STORAGE_NB_SECTOR;
	}

	inline static bool save(const T &obj) {
		debugf("[FlashStorage]: storing object UUID=%d\n", UUID);
		if (isSame(obj)) {
			debugf("[FlashStorage]: not saving as it is unchanged\n");
			return false;
		}

		size_t idx = findFree();

		if (idx == PAGES_PER_SECTOR * PICO_NV_STORAGE_NB_SECTOR) {
			idx = erase();
		}

		uint8_t buffer[PagesPerObject * FLASH_PAGE_SIZE];
		Header  h = {
		     .SizeInPages = PagesPerObject,
		     .Identifier  = UUID,
        };

		memcpy(buffer, &h, sizeof(Header));
		memcpy(buffer + sizeof(Header), &obj, sizeof(Type));
		memset(
		    buffer + sizeof(Header) + sizeof(Type),
		    0x00,
		    PagesPerObject * FLASH_PAGE_SIZE - sizeof(Header) - sizeof(Type)
		);

		flash_range_program(
		    XIP_OFFSET + idx * FLASH_PAGE_SIZE,
		    buffer,
		    PagesPerObject * FLASH_PAGE_SIZE
		);

		debugf("[FlashStorage]: saved at page %d\n", idx);
		return true;
	}

	inline static int erase() {
		debugf("[FlashStorage]: erasing page\n");

		// We need to save all values that will not be affected
		std::map<uint16_t, std::vector<uint8_t>> saved;
		forAllValidPages([&](const Header *h) {
			if (h->Identifier == UUID) {
				return true;
			}
			debugf("[FlashStorage]: Saving UUID=%d\n", h->Identifier);
			saved[h->Identifier].resize(h->SizeInPages * FLASH_PAGE_SIZE);
			memcpy(
			    saved[h->Identifier].data(),
			    reinterpret_cast<const uint8_t *>(h),
			    h->SizeInPages * FLASH_PAGE_SIZE
			);
			return true;
		});
		flash_range_erase(
		    XIP_OFFSET,
		    PICO_NV_STORAGE_NB_SECTOR * FLASH_SECTOR_SIZE
		);
		size_t i = 0;
		for (const auto &[_, data] : saved) {
			auto header = reinterpret_cast<const Header *>(data.data());
			debugf(
			    "[FlashStorage]: Programming UUID=%d at page %d size:%d\n",
			    header->Identifier,
			    i / FLASH_PAGE_SIZE,
			    data.size()
			);

			flash_range_program(XIP_OFFSET + i, data.data(), data.size());
			debugf(
			    "[FlashStorage]: done programming UUID=%d.\n",
			    header->Identifier
			);
			i += data.size();
		}
		debugf("[FlashStorage]: next free page is %d\n", i / FLASH_PAGE_SIZE);
		return i / FLASH_PAGE_SIZE;
	}

	inline static bool isSame(const T &obj) {
		Type saved;
		if (!load(saved)) {
			return false;
		}
		return memcmp(&saved, &obj, sizeof(Type)) == 0;
	}
};

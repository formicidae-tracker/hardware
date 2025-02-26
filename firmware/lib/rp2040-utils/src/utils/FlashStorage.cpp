#include "FlashStorage.hpp"
#include <boards/pico.h>
#include <cstdint>
#include <hardware/flash.h>
#include <pico/flash.h>
#include <pico/types.h>

namespace details {
constexpr static size_t LINE_SIZE = 16;

struct printMemoryArgs {
	const uint8_t *buffer;
	size_t         size;
};

void print4Bytes(const uint8_t *data, size_t size) {
	switch (size) {
	case 0:
		printf("···· ····");
		break;
	case 1:
		printf("%02X·· ····", data[0]);
		break;
	case 2:
		printf("%02X%02X ····", data[0], data[1]);
		break;
	case 3:
		printf("%02X%02X %02X··", data[0], data[1], data[2]);
		break;
	default:
		printf("%02X%02X %02X%02X", data[0], data[1], data[2], data[3]);
	}
}

void printLine(const uint8_t *line, size_t size) {
	printf("| Addr:0x%08x | ", int(line));
	print4Bytes(line, size);
	printf("   ");
	print4Bytes(line + 4, std::max(size - 4, 0U));
	printf("   ");
	print4Bytes(line + 8, std::max(size - 8, 0U));
	printf("   ");
	print4Bytes(line + 12, std::max(size - 12, 0U));
	printf(" |\n");
}

void printMemory(void *params) {
	const printMemoryArgs *args = const_cast<const printMemoryArgs *>(
	    reinterpret_cast<printMemoryArgs *>(params)
	);

	for (size_t i = 0; i < args->size; i += LINE_SIZE) {
		printLine(args->buffer + i, args->size - i);
	}
}

void PrintMemory(const uint8_t *addr, size_t size) {
	printf("Will print 0X%08x - 0X%08x\n", int(addr), int(addr + size));
	printMemoryArgs args{
	    .buffer = addr,
	    .size   = size,
	};
	flash_safe_execute(&printMemory, &args, 1000);
}

void PrintFlashStorage() {
	size_t size = PICO_NV_STORAGE_NB_SECTOR * FLASH_SECTOR_SIZE;
	auto   addr = XIP_BASE + PICO_FLASH_SIZE_BYTES - size;

	PrintMemory((uint8_t *)addr, size);
}

} // namespace details

#include "crc8.h"

uint8_t CRC8_AppendByte(uint8_t crc, uint8_t generator, uint8_t value) {
	crc ^= value;

	for (int i = 0; i < 8; i++) {
		if ((crc & 0x80) != 0) {
			crc = (uint8_t)((crc << 1) ^ generator);
		} else {
			crc = crc << 1;
		}
	}
	return crc;
}

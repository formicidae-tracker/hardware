#include "system_clock.hpp"
#include <avr/io.h>

#define PIN      1
#define DELAY_MS 500

int main() {
	init_system_clock();
	PORTA.DIRSET = _BV(1);
	auto last    = get_absolute_time();

	constexpr auto period_tck = MSToTicks(500U); // 500ms

	for (;;) {
		auto now = get_absolute_time();
		if ((now - last) < period_tck) {
			continue;
		}
		last = now;
		PORTA.OUT ^= _BV(1);
	}
	return 0;
}

#include "system_clock.hpp"
#include <avr/io.h>

#define PIN      1
#define DELAY_MS 500

inline void init() {
	// enable clock change
	CCP               = CCP_IOREG_gc;
	// Enable a 10MHz clock for a 3.3V power supply.
	CLKCTRL.MCLKCTRLB = CLKCTRL_PDIV_2X_gc | CLKCTRL_PEN_bm;
	CCP               = 0; // Protect the clock again.

	init_system_clock();

	PORTA.DIRSET = _BV(1);
}

int main() {
	init();

	auto last = get_absolute_time();

	constexpr auto period_tck = MSToTicks(10U); // 500ms

	for (;;) {
		auto now = get_absolute_time();
		if ((now - last) < period_tck) {
			continue;
		}
		last += period_tck;
		PORTA.OUT ^= _BV(1);
	}
	return 0;
}

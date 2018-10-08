#include "Systime.h"
#include "LEDs.h"

int main() {
	InitLEDs();
	InitSystime();

	Systime_t last = 0;

	while(1) {
		Systime_t now = GetSystime();

		if ( (now-last) >= 500 ) {
			LEDReadyToggle();
		}

	}

	return 0;
}

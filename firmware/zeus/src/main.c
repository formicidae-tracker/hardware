#include "LEDs.h"
#include "stdbool.h"
#include <arke-avr/systime.h>



int main() {

	InitLEDs();

	ArkeInitSystime();

	LEDReadyPulse();

	while(true) {
		ProcessLEDs();
	}

	return 0;
}

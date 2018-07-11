#include <stdbool.h>

#include "Debug.h"
#include "LightManager.h"
#include "ChargeMonitor.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <inttypes.h>
#include <util/delay.h>

int main() {


	InitDebug();
	InitChargeMonitor();
	InitLightManager();

	//Log(0xaa);
	while(true) {
		CMCheckCharge();
		DProcess();
	}

	return 0;
}

#include <stdbool.h>

#include "Debug.h"
#include "LightManager.h"
#include "ChargeMonitor.h"

int main() {


	InitDebug();
	InitChargeMonitor();
	InitLightManager();
	Log(0);

	uint8_t seconds = 0;
	while(true) {
		if ( (LMSystime() & 0x3ff) == 0 ) {
			Log(++seconds);
		}

		DProcess();
	}

	return 0;
}

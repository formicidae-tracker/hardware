#include <stdbool.h>

#include "Debug.h"
#include "LightManager.h"
#include "ChargeMonitor.h"

int main() {


	InitDebug();
	InitChargeMonitor();
	InitLightManager();
	Log(0);

	while(true) {
		if (CMCheckCharge()) {
			LMActivateOutput();
		} else {
			LMDeactivateOutput();
		}

	}

	return 0;
}

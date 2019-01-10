#include "Systime.h"
#include "LEDs.h"
#include "HostLink.h"


#include <yaacl.h>

int main() {
	InitSystime();
	InitLEDs();
	InitHostLink();

	while(1) {
		ProcessHostLink();
		ProcessLEDs();
	}

	return 0;
}

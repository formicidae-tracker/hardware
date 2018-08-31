#include <avr/io.h>

#include "Systime.h"
#include "ModuleManager.h"

int main() {
	InitSystime();
	InitModuleManager();
	Systime_t last = GetSystime();
	uint8_t vis = 0;
	int8_t incr = 1;
	SendToModule(vis,0);
	while(1) {
		ProcessModuleManager();

		Systime_t now =  GetSystime();
		if ( ( now - last ) > 9 ) {
			last = now;
			if (vis == 255) {
				incr = -1;
			}
			if (vis == 0 ) {
				incr = 1;
			}
			vis = vis + incr;
			SendToModule(vis, 0);
		}

	}

	return 0;
}

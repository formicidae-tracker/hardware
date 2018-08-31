#include <avr/io.h>

#include "Systime.h"
#include "ModuleManager.h"

#define DEAD_TIME 30

int main() {
	InitSystime();
	InitModuleManager();
	Systime_t last = GetSystime();
	int16_t vis = 0;
	int8_t incr = 1;
	SendToModule(vis,0);
	while(1) {
		ProcessModuleManager();

		Systime_t now =  GetSystime();
		if ( ( now - last ) >= 10 ) {
			last = now;
			if (vis == (255 + DEAD_TIME)) {
				incr = -1;
			}
			if (vis == (-DEAD_TIME) ) {
				incr = 1;
			}
			vis = vis + incr;
			uint8_t toSend = vis & 0xff;
			if ( vis  < 0 ) {
				toSend = 0;
			} else if ( vis > 255 ) {
				toSend = 0xff;
			}

			SendToModule(toSend, 0);
		}
	}

	return 0;
}

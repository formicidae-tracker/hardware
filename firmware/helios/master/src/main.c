#include <avr/io.h>

#include "Systime.h"


int main() {
	InitSystime();

	DDRD |= _BV(2) | _BV(4);

	Systime_t last = GetSystime();

	while(1) {
		Systime_t now =  GetSystime();

		if ( ( now - last ) > 499 ) {
			last = now;
			PORTD ^=_BV(2) |  _BV(4);
		}

	}



	return 0;
}

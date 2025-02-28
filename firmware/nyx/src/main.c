#include <avr/io.h>
#include <util/delay.h>

#define PIN 1
#define DELAY_MS 500





int main() {

	PORTA.DIRSET = _BV(1);

	while(1) {
		PORTA.OUT ^= _BV(1);
		_delay_ms(DELAY_MS);
	}
	return 0;

}

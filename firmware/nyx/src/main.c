#include <avr/io.h>
#include <util/delay.h>

#define PIN PA1
#define DELAY_MS 500

int main() {

	DDRA = _BV(PIN);

	while(1) {
		PORTA ^= _BV(PIN);
		_delay_ms(DELAY_MS);
	}
	return 0;

}

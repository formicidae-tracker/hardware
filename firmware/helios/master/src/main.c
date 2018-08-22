#include <avr/io.h>

#include <util/delay.h>

int main() {
	DDRD |= _BV(2) | _BV(4);


	do {
		_delay_ms(500);
		PORTD |= _BV(2) | _BV(4);
		_delay_ms(500);
		PORTD &= ~(_BV(2) | _BV(4));

	}while(1);


	return 0;
}

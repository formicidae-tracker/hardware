#include <avr/io.h>
#include <util/delay.h>
int main() {

	DDRF |= _BV(5) | _BV(6);
	DDRB |= _BV(6);
	PORTB |= _BV(6);
	PORTF |= _BV(5) | _BV(6);

	int i = 0;
	while(1) {
		++i;
		_delay_ms(1000);
		PORTF = (i &0x3) << 5;
	}

	return 0;
}

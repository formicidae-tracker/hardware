#include "pico/stdio.h"
#include "pico/time.h"
#include <cstdio>



int main() {
	stdio_init_all();

	auto last = delayed_by_ms(get_absolute_time(), -1000);

	int i = 0;
	while(true) {
		auto now = get_absolute_time();

		if ( absolute_time_diff_us(last, now) < 1000000 ) {
			continue;
		}

		last = delayed_by_ms(last, 1000);
		printf("Coucou %d\n",++i);

	}
}

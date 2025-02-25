#include "Arke.hpp"
#include <utils/Scheduler.hpp>

#include "arke.h"

int main() {
	ArkeInit(ArkeConfig{
	    .PinRX     = 0,
	    .PinTX     = 1,
	    .Class     = ARKE_HELIOS,
	    .ClassMask = ArkeNodeClass(uint32_t(ARKE_HELIOS) & 0x3),
	    .Callback  = [](const ArkeEvent &) {},
	});

	while (true) {
		Scheduler::Work();
	}
	return 0;
}

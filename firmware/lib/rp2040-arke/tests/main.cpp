#include "Arke.hpp"
#include "Scheduler.hpp"

#include "arke.h"

int main() {
	ArkeInit(ArkeConfig{
	    .CanRX     = 0,
	    .CanTX     = 1,
	    .Class     = ARKE_HELIOS,
	    .ClassMask = ArkeNodeClass(uint32_t(ARKE_HELIOS) & 0x3),
	    .ID        = 1,
	    .Callback  = [](const ArkeEvent &) {},
	});

	while (true) {
		Scheduler::Work();
	}
	return 0;
}

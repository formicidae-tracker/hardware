#include "Arke.hpp"

#include <Log.hpp>

void ArkeInit(ArkeConfig &&config) {

	Infof(
	    "Arke initialized with class 0x%x and ID=%d",
	    config.Class,
	    config.ID
	);
}

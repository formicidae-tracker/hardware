#include <stdbool.h>

#include "Debug.h"
#include "LightManager.h"


int main() {
	InitDebug();
	InitLightManager();
	Log(0);

	while(true) {
		//DO something

	}


	return 0;
}

#include <pico/stdlib.h>


#include <utils/Log.hpp>





int main() {
	stdio_init_all();

	Logger::InitLogsOnSecondCore();
#ifndef NDEBUG
	Logger::Get().SetLevel(Logger::Level::DEBUG);
#endif





	return 0;
}

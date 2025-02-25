set(SRC_FILES
	Defer.hpp
	RingBuffer.hpp
	Scheduler.hpp
	Scheduler.cpp
	Queue.hpp
	Log.hpp
	Log.cpp
	FlashStorage.hpp
	FlashStorage.cpp
)

add_library(rp2040-utils INTERFACE)
target_sources(rp2040-utils INTERFACE ${SRC_FILES})

target_link_libraries(
	rp2040-utils INTERFACE pico_stdlib pico_multicore hardware_flash pico_flash
)
target_include_directories(rp2040-utils INTERFACE ${CMAKE_CURRENT_LIST_DIR}/../)

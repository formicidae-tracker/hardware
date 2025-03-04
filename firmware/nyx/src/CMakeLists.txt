set(SRC_FILES
	main.cpp #
	system_clock.cpp #
	system_clock.hpp #
	defer.hpp
)

add_avr_executable(nyx ${SRC_FILES})

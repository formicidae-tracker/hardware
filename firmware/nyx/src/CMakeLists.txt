set(SRC_FILES
	main.cpp #
	system_clock.cpp #
	system_clock.hpp #
	defer.hpp
)

add_avr_executable(nyx ${SRC_FILES})

set(PYMCUPROG_OPTIONS -d attiny202 -t uart -u /dev/ttyUSB1)

add_custom_target(nyx-ping ${PYMCUPROG_EXECUTABLE} ${PYMCUPROG_OPTIONS} ping)
add_custom_target(
	nyx-upload
	${PYMCUPROG_EXECUTABLE}
	${PYMCUPROG_OPTIONS}
	write
	-f
	nyx-attiny202.hex
	--erase
	--verify
	DEPENDS nyx-attiny202.hex
)

set(SRC_FILES
	# main.cpp #
	# system_clock.cpp #
	# system_clock.hpp #
	# defer.hpp
	main.c
)

add_avr_executable(phaeton ${SRC_FILES})

set(PYMCUPROG_OPTIONS -d attiny202 -t uart -u ${PYMCUPROG_PORT})

add_custom_target(
	phaeton-ping ${PYMCUPROG_EXECUTABLE} ${PYMCUPROG_OPTIONS} ping
)
add_custom_target(
	phaeton-upload
	${PYMCUPROG_EXECUTABLE}
	${PYMCUPROG_OPTIONS}
	write
	-f
	phaeton-attiny202.hex
	--erase
	--verify
	DEPENDS phaeton-attiny202.hex
)

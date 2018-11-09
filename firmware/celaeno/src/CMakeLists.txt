set(SRC_FILES main.c
	          Systime.c
	          LEDs.c)

set(HDR_FILES Systime.h
	          LEDs.h)

add_avr_executable(celaeno ${SRC_FILES} ${HDR_FILES})

avr_target_link_libraries(celaeno libarke yaail)

set(SRC_FILES main.c
	          LEDs.c)

set(HDR_FILES LEDs.h)


add_avr_executable(zeus ${SRC_FILES} ${HDR_FILES} )

avr_target_link_libraries(zeus libarke yaail)

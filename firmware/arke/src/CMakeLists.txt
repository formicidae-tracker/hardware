set(SRC_FILES main.c
	          LEDs.c
	          Systime.c
	          HostLink.c)

set(HDR_FILES LEDs.h
	          Systime.h
	          HostLink.h)


add_avr_executable(arke ${SRC_FILES} ${HDR_FILES})

avr_target_link_libraries(arke yaacl)

set(SRC_FILES main.c
	          ModuleManager.c)

set(HDR_FILES ModuleManager.h)

add_avr_executable(helios-master ${SRC_FILES} ${HDR_FILES})

avr_target_link_libraries(helios-master libarke yaacl)

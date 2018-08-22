set(SRC_FILES main.c
	          Systime.c
	          ModuleManager.c)

set(HDR_FILES Systime.h
	          ModuleManager.h)

add_avr_executable(helios-master ${SRC_FILES} ${HDR_FILES})

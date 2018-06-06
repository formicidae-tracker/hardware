set(SRC_FILES main.c
              Debug.c)

set(HDR_FILES Debug.h
              )

add_avr_executable(helios-module ${SRC_FILES} ${HDR_FILES})

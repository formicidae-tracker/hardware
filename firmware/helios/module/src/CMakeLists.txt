set(SRC_FILES main.c
              Debug.c
              LightManager.c)

set(HDR_FILES Debug.h
              LightManager.h)

add_avr_executable(helios-module ${SRC_FILES} ${HDR_FILES})

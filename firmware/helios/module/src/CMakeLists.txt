set(SRC_FILES main.c
              Debug.c
              LightManager.c
              ChargeMonitor.c
              SerialInterface.c)

set(HDR_FILES Debug.h
              LightManager.h
              ChargeMonitor.h
              SerialInterface.h)


add_avr_executable(helios-module ${SRC_FILES} ${HDR_FILES})

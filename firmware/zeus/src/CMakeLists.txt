set(SRC_FILES main.c
	          LEDs.c
	          FanControl.c
	          Heaters.c
	          ClimateController.c
	          Sensors.c)

set(HDR_FILES LEDs.h
	          config.h
	          FanControl.c
	          Heaters.h
	          ClimateController.h
	          Sensors.h)


add_avr_executable(zeus ${SRC_FILES} ${HDR_FILES} )

avr_target_link_libraries(zeus libarke yaail)

set(SRC_FILES
	main.c #
	LEDs.c #
	FanControl.c #
	PIDController.c #
	ClimateController.c
	Sensors.c
)

set(HDR_FILES
	LEDs.h #
	config.h #
	FanControl.c #
	PIDController.h #
	ClimateController.h #
	Heaters.h #
	Sensors.h
)

if(ZEUS_USE_INTERNAL_HEATER)
	list(APPEND SRC_FILES Heaters_internal.c)
else(ZEUS_USE_INTERNAL_HEATER)
	list(APPEND SRC_FILES Heaters_notus.c)
endif(ZEUS_USE_INTERNAL_HEATER)

add_avr_executable(zeus ${SRC_FILES} ${HDR_FILES})

avr_target_link_libraries(zeus libarke yaail)

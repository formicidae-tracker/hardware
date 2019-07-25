set(SRC_FILES Systime.c
              LightManager.c
              ChargeMonitor.c
              SerialInterface.c)

set(HDR_FILES Systime.h
              LightManager.h
              ChargeMonitor.h
              SerialInterface.h)

option(HELIOS_MODULE_TEST_FIRMWARE "Build a test firmware for helios module")

if(NOT HELIOS_MODULE_TEST_FIRMWARE)
	list(APPEND SRC_FILES main.c)
else(NOT HELIOS_MODULE_TEST_FIRMWARE)
	message(WARNING "################################################################################\nBuilding test firmware\n################################################################################")
	list(APPEND SRC_FILES main-test.c)
endif(NOT HELIOS_MODULE_TEST_FIRMWARE)


add_avr_executable(helios-module ${SRC_FILES} ${HDR_FILES})

set(SRC_FILES main.c LEDs.c FanControl.c)

set(HDR_FILES config.h LEDs.h FanControl.h)

add_avr_executable(celaeno ${SRC_FILES} ${HDR_FILES})

avr_target_link_libraries(celaeno libarke yaail)

remove_definitions("-DF_CPU=${MCU_SPEED}")

add_avr_executable(blink main-blink.c)

avr_target_compile_definitions(blink PRIVATE "-DF_CPU=8000000")

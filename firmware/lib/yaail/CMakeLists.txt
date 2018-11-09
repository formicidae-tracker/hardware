set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/../../deps/cmake-avr/generic-gcc-avr.cmake)


set(AVR_UPLOADTOOL avrdude)
set(AVR_PROGRAMMER arduinoisp)
set(AVR_UPLOADTOOL_PORT usb)
set(AVR_UPLOADTOOL_OPTIONS -C ${CMAKE_CURRENT_SOURCE_DIR}/../../avrdude.conf)

set(AVR_MCU at90can128)
set(AVR_L_FUSE 0xff)
set(AVR_H_FUSE 0xd9)
set(AVR_E_FUSE 0x01)
set(MCU_SPEED "16000000L")

project(yaail C CXX)
cmake_minimum_required(VERSION 3.2)

add_definitions("-DF_CPU= ${MCU_SPEED}")
add_compile_options("-fpack-struct")
add_compile_options("-fshort-enums")
add_compile_options("-Wall")
add_compile_options("-Werror")
add_compile_options("-pedantic")
add_compile_options("-pedantic-errors")
add_compile_options("-funsigned-char")
add_compile_options("-funsigned-bitfields")
add_compile_options("-ffunction-sections")

set(CMAKE_C_STANDARD 99)

include_directories(${AVR_GCC_INCLUDE} ${PROJECT_SOURCE_DIR}/src)

set(HDR_FILES src/yaail.h
	          )

set(SRC_FILES src/yaail-src-all.c)

add_avr_library(yaail ${SRC_FILES} ${HDR_FILES})


find_program(CLDOC_EXECUTABLE cldoc)

if(CLDOC_EXECUTABLE)
	add_custom_target(doc ${CLDOC_EXECUTABLE} "generate"
		"-D__AVR_ATmega16M1__"
		"--"
		"--language" "c"
		"--output" "${PROJECT_BINARY_DIR}/doc"
		${HDR_FILES}
		COMMENT "Generating doc in ${PROJECT_BINARY_DIR}/doc. Press Ctrl-c to end"
		WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})

	add_custom_target(serve-doc ${CLDOC_EXECUTABLE} serve ./doc
		COMMENT "Serving documentation . Press Ctrl-c to end"
		WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
		DEPENDS doc)

endif(CLDOC_EXECUTABLE)

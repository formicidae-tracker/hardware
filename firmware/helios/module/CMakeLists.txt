set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/../../deps/cmake-avr/generic-gcc-avr.cmake)

set(AVR_UPLOADTOOL avrdude)
set(AVR_PROGRAMMER avrispmkii)
set(AVR_UPLOADTOOL_PORT usb)
set(AVR_UPLOADTOOL_OPTIONS -C ${CMAKE_CURRENT_SOURCE_DIR}/../../avrdude.conf -B 8)

set(AVR_MCU atmega328pb)
set(AVR_L_FUSE 0xff)
set(AVR_H_FUSE 0xd9)
set(AVR_E_FUSE 0xf4)
set(MCU_SPEED "20000000L")


project(helio-module C CXX)
cmake_minimum_required(VERSION 3.2)


add_definitions("-DF_CPU=${MCU_SPEED}" )
set(ATMEL_DEFINITION_PACK  CACHE STRING "Adds an Atmel Definion pack for new devices")
if(ATMEL_DEFINITION_PACK)
	set(CMAKE_C_FLAGS "-B ${ATMEL_DEFINITION_PACK}/gcc/dev/atmega328pb/")
	include_directories(${ATMEL_DEFINITION_PACK}/include)
endif(ATMEL_DEFINITION_PACK)
add_compile_options("-fpack-struct")
add_compile_options("-fshort-enums")
add_compile_options("-Wall")
add_compile_options("-Werror")
add_compile_options("-pedantic")
add_compile_options("-pedantic-errors")
add_compile_options("-funsigned-char")
add_compile_options("-funsigned-bitfields")
add_compile_options("-ffunction-sections")
add_compile_options("-std=c99")

include_directories(${PROJECT_SOURCE_DIR}/../)

set(CMAKE_C_STANDARD 99)

add_subdirectory(src)

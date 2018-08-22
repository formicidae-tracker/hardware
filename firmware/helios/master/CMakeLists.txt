set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/../../deps/cmake-avr/generic-gcc-avr.cmake)


set(AVR_UPLOADTOOL avrdude)
set(AVR_PROGRAMMER arduinoisp)
set(AVR_UPLOADTOOL_PORT usb)
set(AVR_UPLOADTOOL_OPTIONS -C ${CMAKE_CURRENT_SOURCE_DIR}/../../avrdude.conf)

set(AVR_MCU atmega16m1)
set(AVR_L_FUSE 0xff)
set(AVR_H_FUSE 0xd9)
set(AVR_E_FUSE 0x01)
set(MCU_SPEED "16000000L")

project(helios-master C CXX)
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

include_directories(${AVR_GCC_INCLUDE})

add_subdirectory(src)

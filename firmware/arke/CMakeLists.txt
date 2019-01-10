set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/../deps/cmake-avr/generic-gcc-avr.cmake)


set(AVR_UPLOADTOOL avrdude)
set(AVR_PROGRAMMER avrispmkii)
#set(AVR_PROGRAMMER arduinoisp)
set(AVR_UPLOADTOOL_PORT usb)
set(AVR_UPLOADTOOL_OPTIONS -C ${CMAKE_CURRENT_SOURCE_DIR}/../avrdude.conf)

set(AVR_MCU atmega16m1)
set(AVR_L_FUSE 0xff)
set(AVR_H_FUSE 0xd9)
set(AVR_E_FUSE 0xf6)
set(MCU_SPEED "16000000L")

cmake_policy(SET CMP0048 NEW)
project(arke LANGUAGES C CXX
             VERSION 0.1)
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

set(YAACL_ROOT ${PROJECT_SOURCE_DIR}/../lib/yaacl)
set(YAACL_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/../lib/yaacl/src)
set(YAACL_SRC_FILES ${PROJECT_SOURCE_DIR}/../lib/yaacl/src/yaacl-src-all.c)

include_directories(${AVR_GCC_INCLUDE}
	                ${PROJECT_SOURCE_DIR}/../
	                ${YAACL_INCLUDE_DIR}
	                ${PROJECT_SOURCE_DIR}/include)

add_definitions("-DARKE_FW_MAJOR=${PROJECT_VERSION_MAJOR}"
	"-DARKE_FW_MINOR=${PROJECT_VERSION_MINOR}")

add_avr_library(yaacl ${YAACL_SRC_FILES})

add_subdirectory(src)

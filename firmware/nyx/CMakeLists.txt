set(DEPS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../deps)
set(CMAKE_TOOLCHAIN_FILE ${DEPS_DIR}/cmake-avr/generic-gcc-avr.cmake)

set(AVR_MCU attiny202)
set(AVR_SIZE_ARGS -B)

cmake_minimum_required(VERSION 3.12)
project(
	nyx
	LANGUAGES C CXX
	VERSION 0.1
)

if(CMAKE_C_COMPILER_VERSION VERSION_LESS 13)
	message(FATAL_ERROR "Only made for gcc-avr >= 13")
endif()

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED 1)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED 1)

set(MCU_SPEED "10000000L")

add_definitions("-DF_CPU=${MCU_SPEED}")

add_subdirectory(src)

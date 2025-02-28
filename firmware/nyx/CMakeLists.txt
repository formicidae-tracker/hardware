set(DEPS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../deps)
set (CMAKE_TOOLCHAIN_FILE ${DEPS_DIR}/cmake-avr/generic-gcc-avr.cmake)

set(AVR_MCU attiny20)

cmake_minimum_required(VERSION 3.12)
project(nyx LANGUAGES C CXX VERSION 0.1)


set(CMAKE_C_STANDARD 99)


set(MCU_SPEED "10000000L")

add_definitions("-DF_CPU=${MCU_SPEED}")

add_subdirectory(src)

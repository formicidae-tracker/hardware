cmake_minimum_required(VERSION 3.17)

set(FAMILY rp2040)
set(BOARD pico_sdk)

set(DEPS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../deps)
set(LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../lib)

set(PICOTOOL_FETCH_FROM_GIT_PATH ${DEPS_DIR}/picotool)

include(${DEPS_DIR}/pico-sdk/pico_sdk_init.cmake)

project(notus)

include(${DEPS_DIR}/cmake/OpenOCD.cmake)

set(PICO_CXX_ENABLE_EXCEPTIONS 1)

pico_sdk_init()

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED 1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED 1)

add_subdirectory(${LIB_DIR}/rp2040-utils
                 ${CMAKE_CURRENT_BINARY_DIR}/lib/rp2040-utils)

include(FetchContent)
FetchContent_Declare(
  can2040
  GIT_REPOSITORY https://github.com/formicidae-tracker/can2040.git
  GIT_TAG origin/dev/cmake)

FetchContent_MakeAvailable(can2040)

add_subdirectory(src)

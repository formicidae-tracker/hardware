cmake_minimum_required(VERSION 3.17)

set(FAMILY rp2040)
set(BOARD pico_sdk)

set(DEPS_DIR ${CMAKE_CURRENT_LIST_DIR}/../deps)
set(LIB_DIR ${CMAKE_CURRENT_LIST_DIR}/../lib)

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

include(FetchContent)
FetchContent_Declare(rp2040-utils URL ${LIB_DIR}/rp2040-utils)
FetchContent_Declare(rp2040-arke URL ${LIB_DIR}/rp2040-arke)
FetchContent_Declare(
	fort-arke
	GIT_REPOSITORY https://github.com/formicidae-tracker/libarke
	GIT_TAG origin/dev/rp2040
)

FetchContent_MakeAvailable(rp2040-utils rp2040-arke fort-arke)

add_subdirectory(src)

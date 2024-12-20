cmake_minimum_required(VERSION 3.13...3.27)

if(NOT DEPS_DIR)
	set(DEPS_DIR ${CMAKE_CURRENT_LIST_DIR}/../../deps)
endif(NOT DEPS_DIR)

if(NOT LIB_DIR)
	set(LIB_DIR ${CMAKE_CURRENT_LIST_DIR}/../../lib)
endif(NOT LIB_DIR)

if(NOT COMMAND pico_sdk_init)

	include(${DEPS_DIR}/pico-sdk/pico_sdk_init.cmake)
endif(NOT COMMAND pico_sdk_init)

project(
	arke-rp2040
	VERSION 0.1.0
	LANGUAGES C CXX
)

if(NOT CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
	set(IS_IMPORTED 1)
endif(NOT CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)

if(IS_IMPORTED)
	if(NOT PICO_SDK_PATH)
		message(
			FATAL_ERROR "This project must be included after pico_sdk_init()"
		)
	endif(NOT PICO_SDK_PATH)
else(IS_IMPORTED)
	set(FAMILY rp2040)
	set(BOARD pico_sdk)
	pico_sdk_init()

	set(CMAKE_C_STANDARD 11)
	set(CMAKE_C_STANDARD_REQUIRED 1)

	set(CMAKE_CXX_STANDARD 17)
	set(CMAKE_CXX_STANDARD_REQUIRED 1)

endif(IS_IMPORTED)

include(FetchContent)
# FetchContent_Declare( can2040 GIT_REPOSITORY
# https://github.com/formicidae-tracker/can2040.git GIT_TAG origin/dev/cmake )

FetchContent_Declare(rp2040-utils SOURCE_DIR ${LIB_DIR}/rp2040-utils)

FetchContent_Declare(
	libarke
	GIT_REPOSITORY https://github.com/formicidae-tracker/libarke.git
	GIT_TAG dev/rp2040
)

FetchContent_MakeAvailable(rp2040-utils libarke)

add_subdirectory(src)

if(NOT IS_IMPORTED)
	add_subdirectory(tests)

endif(NOT IS_IMPORTED)

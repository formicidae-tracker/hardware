cmake_minimum_required(VERSION 3.13...3.27)

if(NOT COMMAND pico_sdk_init)
	set(DEPS_DIR ${CMAKE_CURRENT_LIST_DIR}/../../deps)
	include(${DEPS_DIR}/pico-sdk/pico_sdk_init.cmake)
endif(NOT COMMAND pico_sdk_init)

project(
	rp2040-utils
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

	include(${DEPS_DIR}/cmake/OpenOCD.cmake)

	set(FAMILY rp2040)
	set(BOARD pico_sdk)
	pico_sdk_init()

	set(CMAKE_C_STANDARD 11)
	set(CMAKE_C_STANDARD_REQUIRED 1)

	set(CMAKE_CXX_STANDARD 17)
	set(CMAKE_CXX_STANDARD_REQUIRED 1)

endif(IS_IMPORTED)

add_subdirectory(src/utils)

if(NOT IS_IMPORTED)
	add_executable(test_compilation tests/main.cpp)

	target_link_libraries(test_compilation rp2040-utils)

	add_executable(test_storage tests/storage.cpp)

	target_link_libraries(test_storage rp2040-utils)

	pico_add_extra_outputs(test_storage)

	add_openocd_upload_target(TARGET test_storage)

endif(NOT IS_IMPORTED)

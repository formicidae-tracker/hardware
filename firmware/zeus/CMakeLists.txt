set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/../deps/cmake-avr/generic-gcc-avr.cmake)


set(AVR_UPLOADTOOL avrdude)
set(AVR_PROGRAMMER avrispmkii)
set(AVR_UPLOADTOOL_PORT usb)
set(AVR_UPLOADTOOL_OPTIONS -C ${CMAKE_CURRENT_SOURCE_DIR}/../avrdude.conf)

set(AVR_MCU at90can32)
set(AVR_L_FUSE 0xde)
set(AVR_H_FUSE 0x99)
set(AVR_E_FUSE 0xfd)
set(MCU_SPEED "16000000L")

cmake_policy(SET CMP0048 NEW)
project(zeus LANGUAGES C CXX
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
set(YAACL_INCLUDE_DIR ${YAACL_ROOT}/src)
set(YAACL_SRC_FILES ${YAACL_ROOT}/src/yaacl-src-all.c)
set(YAAIL_ROOT ${PROJECT_SOURCE_DIR}/../lib/yaail)
set(YAAIL_INCLUDE_DIR ${YAAIL_ROOT}/src)
set(YAAIL_SRC_FILES ${YAAIL_ROOT}/src/yaail-src-all.c)
set(LIBARKE_ROOT ${PROJECT_SOURCE_DIR}/../deps/fort-libarke)
set(LIBARKE_INCLUDE_DIR ${LIBARKE_ROOT}/include)
set(LIBARKE_SRC_FILES ${LIBARKE_ROOT}/src-avr/arke-src-all.c)

set(EMC_IC_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/../lib/emc-ic)

include_directories(${AVR_GCC_INCLUDE}
	                ${PROJECT_SOURCE_DIR}/../
	                ${YAACL_INCLUDE_DIR}
	                ${YAAIL_INCLUDE_DIR}
	                ${LIBARKE_INCLUDE_DIR}
	                ${PROJECT_SOURCE_DIR}/include
	                ${EMC_IC_INCLUDE_DIR})

add_definitions("-DARKE_MY_FW_MAJOR_VERSION=${PROJECT_VERSION_MAJOR}"
	            "-DARKE_MY_FW_MINOR_VERSION=${PROJECT_VERSION_MINOR}"
	            "-DARKE_MY_CLASS=0x38"
	            "-DARKE_MY_SUBID=0x01"
	            "-DARKE_MY_SIZE=8"
	            "-DFAN_CONTROL_STALL_THRESHOLD=0x7f"
	            )

add_avr_library(yaacl ${YAACL_SRC_FILES})

add_avr_library(libarke ${LIBARKE_SRC_FILES})

add_avr_library(yaail ${YAAIL_SRC_FILES})

avr_target_link_libraries(libarke yaacl)

add_subdirectory(src)

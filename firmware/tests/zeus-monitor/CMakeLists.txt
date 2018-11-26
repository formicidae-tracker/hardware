cmake_policy(SET CMP0048 NEW)
project(ZeusMonitor LANGUAGES C CXX
	                VERSION 0.1)

cmake_minimum_required(VERSION 3.2)

set(CMAKE_CXX_STANDARD 11)

find_package(PkgConfig REQUIRED)
pkg_check_modules(DEPS REQUIRED biorob-cpp-0.4>=0.4.3
 	                            libglog)

set(ARKE_ROOT ${PROJECT_SOURCE_DIR}/../../deps/fort-libarke/)
set(ARKE_INCLUDE_DIR ${ARKE_ROOT}/include)


include_directories(${DEPS_INCLUDE_DIRS} ${ARKE_INCLUDE_DIR})

add_subdirectory(src)

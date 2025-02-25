set(SRC_FILES Arke.hpp Arke.cpp)

add_library(rp2040-arke INTERFACE)
target_sources(rp2040-arke INTERFACE ${SRC_FILES})

target_link_libraries(
	rp2040-arke INTERFACE rp2040-utils arke pico_stdlib can2040::can2040
)
target_include_directories(
	rp2040-arke INTERFACE ${CMAKE_CURRENT_LIST_DIR} S{CMAKE_CURRENT_BINARY_DIR}
)

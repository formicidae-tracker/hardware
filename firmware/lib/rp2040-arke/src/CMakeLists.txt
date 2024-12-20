set(SRC_FILES Arke.hpp Arke.cpp canbus.cpp canbus.h)

add_library(rp2040-arke INTERFACE)
target_sources(rp2040-arke INTERFACE ${SRC_FILES})

pico_generate_pio_header(rp2040-arke ${CMAKE_CURRENT_LIST_DIR}/canbus.pio)

target_link_libraries(
	rp2040-arke INTERFACE rp2040-utils arke hardware_pio pico_stdlib
)
target_include_directories(
	rp2040-arke INTERFACE ${CMAKE_CURRENT_LIST_DIR} S{CMAKE_CURRENT_BINARY_DIR}
)

set(SRC_FILES #
	main.cpp #
	LED.cpp
)

set(HDR_FILES #
	LED.hpp #
)

configure_file(config_helios.in config_helios.hpp)

add_executable(helios ${SRC_FILES})

target_compile_definitions(helios PUBLIC BOARD=pico_sdk)
target_include_directories(helios PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

target_link_libraries(helios pico_stdlib rp2040-utils rp2040-arke hardware_pwm)

pico_add_extra_outputs(helios)

add_openocd_upload_target(TARGET helios)

add_executable(notus main.cpp)
target_compile_definitions(notus PUBLIC BOARD=pico)

target_link_libraries(notus pico_stdlib can2040::can2040)

pico_add_extra_outputs(notus)

add_openocd_upload_target(TARGET notus)

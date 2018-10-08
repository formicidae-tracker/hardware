set(SRC_FILES main.c)

set(HDR_FILES )


add_avr_executable(arke ${SRC_FILES} ${HDR_FILES})

avr_target_link_libraries(arke yaacl)

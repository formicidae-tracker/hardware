set(SRC_FILES main.cpp
	          CAN.cpp
	          )

set(HDR_FILES CAN.hpp)

add_executable(zeus-monitor ${SRC_FILES} ${HDR_FILES})

target_link_libraries(zeus-monitor ${DEPS_LDFLAGS})

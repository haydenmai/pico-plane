# CMakeList.txt for MSP
#   Build a library (`msp_cam`) which exposes the header files as "msp_cam/*.h"

include_directories(msp_cam/include)
file(GLOB MY_SOURCES "src/*.cpp")

add_library(msp_cam STATIC ${MY_SOURCES})

# Inherit include paths from SDK libraries
target_link_libraries(msp_cam
	PRIVATE pico_stdlib 
	PRIVATE hardware_uart
)

target_include_directories(msp_cam PUBLIC include)
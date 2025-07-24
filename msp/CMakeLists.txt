# CMakeList.txt for MSP
#   Build a library (`msp`) which exposes the header files as "msp/*.h"

include_directories(msp/include)
file(GLOB MY_SOURCES "src/*.cpp")

add_library(msp STATIC ${MY_SOURCES})

# Inherit include paths from SDK libraries
target_link_libraries(msp
	PRIVATE pico_stdlib 
	PRIVATE hardware_uart
)

target_include_directories(msp PUBLIC include)
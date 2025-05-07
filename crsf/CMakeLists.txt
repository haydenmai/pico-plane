# CMakeList.txt for CRSF
#   Build a library (`crsf`) which exposes the header files as "crsf/*.h"

include_directories(crsf/include)
file(GLOB MY_SOURCES "src/*.cpp")

add_library(crsf STATIC ${MY_SOURCES})

# Inherit include paths from SDK libraries
target_link_libraries(crsf
	PRIVATE hardware_pwm
)

target_include_directories(crsf PUBLIC include)
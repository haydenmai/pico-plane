# CMakeList.txt for HAL
#   Build a library (`hal`) which exposes the header files as "hal/*.h"
#   Use header as: #include "hal/button.h"

include_directories(hal/include)
file(GLOB MY_SOURCES "src/*.cpp")

add_library(hal STATIC ${MY_SOURCES})

# Inherit include paths from SDK libraries
target_link_libraries(hal
    PRIVATE pico_stdlib
    PRIVATE pico_cyw43_arch_none
	PRIVATE hardware_adc
	PRIVATE hardware_gpio
	PRIVATE hardware_pwm
)

target_include_directories(hal PUBLIC include)
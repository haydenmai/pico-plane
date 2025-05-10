# Add headers and source files
include_directories(include)
file(GLOB MY_SOURCES "src/*.cpp")
add_executable(${APP_TARGET} ${MY_SOURCES})

# Link required libraries
target_link_libraries(${APP_TARGET}
    PRIVATE hal						# Make use of the HAL library
    PRIVATE pico_stdlib				# Add the standard library to the build
    PRIVATE pico_cyw43_arch_none
	PRIVATE hardware_adc
)

# Set program name and version (used by tools like picotool)
pico_set_program_name(${APP_TARGET} "${APP_TARGET}")
pico_set_program_version(${APP_TARGET} "0.1")

# Modify the below lines to enable/disable output over UART/USB
# Debug output with USB uses +6K memory
pico_enable_stdio_uart(${APP_TARGET} 0)
pico_enable_stdio_usb(${APP_TARGET}  0)

pico_set_float_implementation(${APP_TARGET} pico) 	# Default, but explicit declaration
#pico_set_binary_type(${APP_TARGET} no_flash) 		# Run from RAM
pico_add_extra_outputs(${APP_TARGET}) 				# Generate .elf, .uf2, etc.

# Place output in top-level build directory for flash onto the Pico
set_target_properties(${APP_TARGET} PROPERTIES 
	RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)
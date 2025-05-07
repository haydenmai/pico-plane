# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)

# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.1.1)
set(toolchainVersion 14_2_Rel1)
set(picotoolVersion 2.1.1)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================

set(PICO_BOARD pico_w CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

# Setup Project
project(pico_plane
		DESCRIPTION "A controllable plane using the Raspberry Pi Pico W"
		VERSION 	0.1
		LANGUAGES 	C CXX ASM)

set(APP_TARGET pico_plane)

# Configure compiling options
set(PICO_CXX_ENABLE_EXCEPTIONS 1)
set(PICO_CXX_ENABLE_RTTI 1)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# From https://github.com/xiphonics/picoTracker/blob/master/sources/CMakeLists.txt
add_compile_options(
  -Werror
  -Wall
  -fstack-usage
  -fcallgraph-info=su # include above info in callgraph
  -Wstack-usage=2048  # allows high watermark at the time this option was added TODO: reduce this for safety and/or reduce stack size
)

# Folders to build 
add_subdirectory(hal)
add_subdirectory(app)
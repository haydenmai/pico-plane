# CRSF
This folder contains the CRSF protocol implementations for the Pi Pico. 

## Portability
It is meant to be portable and should work with other Pico's C/C++ projects using the Pico's SDK.
- Drag and drop this folder into the project.
- Add `add_subdirectory(crsf)` to the project root folder's CMakeLists.txt
- Use `#include "crsf/crsf.h` in the code

## Todos
Serveral things are to be looked at after implementation of everything else
- Payload often fails due to bytes being overwritten
- The number of 

## Credits
This code is a modified version of https://github.com/britannio/pico_crsf/tree/main to make it work with C++.
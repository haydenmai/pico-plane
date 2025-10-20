/**
 * @file flight_control.h
 * @brief Controls the direction and speed of the plane.
 * @author Benley Hsiang
 * @date Oct-19-2025
 */

#ifndef FLIGHT_CONTROL_H_
#define FLIGHT_CONTROL_H_

#include "angle_control.h"
#include "speed_control.h"

namespace FlightController {
    void init();
    void cleanup();

    /**
     * @brief Process data from the FlightData module & apply controls to hardware.
     *
     */
    void process_data() noexcept;

}; // namespace FlightController

#endif

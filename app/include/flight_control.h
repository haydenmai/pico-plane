/**
 * @file flight_control.h
 * @brief Controls the direction and speed of the plane.
 * @author Benley Hsiang
 * @date Oct-04-2025
 */

#ifndef FLIGHT_CONTROL_H_
#define FLIGHT_CONTROL_H_

#include "angle_control.h"
#include "speed_control.h"

namespace FlightController {
    /** @brief Maximum throttle percentage of the motors. */
    constexpr int THROTTLE_LIMIT {10};

    /** @brief Lower and upper limits of the angles the control surfaces can turn. */
    // TODO: Values are not in range, need to adjust them
    constexpr AngleController::TurningRange AILERON_RANGE {150, 180};
    constexpr AngleController::TurningRange RUDDER_RANGE {150, 180};
    constexpr AngleController::TurningRange ELEVATOR_RANGE {0, 30};
    // TODO: Values are not in range, need to adjust them

    void init();
    void cleanup();

    /**
     * @brief Process data from the FlightData module & apply controls to hardware.
     *
     */
    void process_data() noexcept;

}; // namespace FlightController

#endif

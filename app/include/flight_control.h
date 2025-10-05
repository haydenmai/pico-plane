/**
 * @file flight_control.h
 * @brief Controls the direction and speed of the plane.
 * @author Benley Hsiang
 * @date Oct-04-2025
 */

#ifndef FLIGHT_CONTROL_H_
#define FLIGHT_CONTROL_H_

#include "angle_control.h"
#include "flight_control.h"
#include "speed_control.h"

namespace FlightController {
void init();
void cleanup();

/**
 * @brief Changes the speed of the plane.
 * @param percent Throttle percentage of the the plane's motors.
 */
void changeSpeed(int percent) noexcept;

/**
 * @brief Turns one of the plane's control surfaces.
 * @param controlType Ailerons, rudder, or elevator.
 * @param angle Angle in degrees to turn to. Resting position is 90 degrees.
 */
void changeAngle(AngleController::ControlType controlType, int angle) noexcept;

/** @brief Maximum throttle percentage of the motors. */
int throttleLimit {10};

SpeedController speedCTRL_ {throttleLimit};

/** @brief Lower and upper limits of the angles the control surfaces can turn. */
AngleController::TurningRange aileronRange {70, 110};
AngleController::TurningRange rudderRange {70, 110};
AngleController::TurningRange elevatorRange {70, 110};

AngleController angleCTRL_ {aileronRange, rudderRange, elevatorRange};
}; // namespace FlightController

#endif

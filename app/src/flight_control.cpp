/**
 * @file flight_control.cpp
 * @brief Controls the direction and speed of the plane.
 * @author Benley Hsiang
 * @date Oct-04-2025
 */

#include "flight_control.h"
#include <cassert>

namespace FlightController {

static bool isInitialized_ = false;

void init()
{
    assert(!isInitialized_);
    isInitialized_ = true;
}

void cleanup()
{
    assert(isInitialized_);
    isInitialized_ = false;
}

void changeSpeed(int percent) noexcept
{
    assert(isInitialized_);
    speedCTRL_.setSpeed(percent);
}

void changeAngle(AngleController::ControlType controlType, int angle) noexcept
{
    assert(isInitialized_);
    AngleController::setAngle(controlType, angle);
}

/** @brief Maximum throttle percentage of the motors. */
int throttleLimit {10};

SpeedController speedCTRL_ {throttleLimit};

/** @brief Lower and upper limits of the angles the control surfaces can turn. */
AngleController::TurningRange aileronRange {70, 110};
AngleController::TurningRange rudderRange {70, 110};
AngleController::TurningRange elevatorRange {70, 110};

} // namespace FlightController

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
    angleCTRL_.setAngle(controlType, angle);
}

} // namespace FlightController

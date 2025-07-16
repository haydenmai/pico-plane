/**
 * @file speed_control.cpp
 * @brief Controls the speed of the plane.
 * @author Benley Hsiang
 * @date Jul-16-2025
 */

#include "speed_control.h"

#include <stdio.h> // Remove later if not needed

SpeedController::SpeedController(int throttleLim) { setThrottleLim(throttleLim); }

SpeedController::~SpeedController() {}

void SpeedController::setThrottleLim(int percent) noexcept
{
    if (percent >= MotorEsc::MIN_THROT && percent <= MotorEsc::MAX_THROT) {
        throttleLim_ = percent;

    } else {
        printf("Error: Illegal value for throttle limit.\n");
        // TODO: Replace this with exception handling eventually.
    }
}

[[nodiscard]] int SpeedController::getThrottleLim(void) const noexcept
{
    return throttleLim_;
}

void SpeedController::setSpeed(int percent) noexcept
{
    // If percent is out of bounds, do nothing
    if (percent >= MotorEsc::MIN_THROT && percent <= throttleLim_) {
        esc_.setSpeed(percent);
    }
}

[[nodiscard]] int SpeedController::getSpeed(void) const noexcept
{
    return esc_.getSpeed();
}

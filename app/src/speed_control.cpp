/**
 * @file speed_control.cpp
 * @brief Controls the speed of the plane.
 * @author Benley Hsiang
 * @date Jul-09-2025
 */

#include "speed_control.h"

#include "hal/motor_esc.h"

#include <stdio.h> // Remove later if not needed

SpeedController::SpeedController(int throttleLim)
{
    if (throttleLim >= MotorEsc::MIN_THROT && throttleLim <= MotorEsc::MAX_THROT) {
        throttleLim_ = throttleLim;

    } else {
        printf("Error: Illegal value for throttle limit.\n");
        // TODO: Replace this with exception handling eventually.
    }
}

void SpeedController::setThrottleLim(int limit) noexcept
{
    if (limit >= MotorEsc::MIN_THROT && limit <= MotorEsc::MAX_THROT) {
        throttleLim_ = limit;

    } else {
        printf("Error: Illegal value for throttle limit.\n");
        // TODO: Replace this with exception handling eventually.
    }
}

[[nodiscard]] int SpeedController::getThrottleLim(void) const noexcept
{
    return throttleLim_;
}

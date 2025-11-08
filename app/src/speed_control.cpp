/**
 * @file speed_control.cpp
 * @brief Controls the speed of the plane.
 * @author Benley Hsiang
 * @date Nov-07-2025
 */

#include "speed_control.h"
#include "flight_config.h"

#include <cassert>
#include <stdio.h> // Remove later if not needed

namespace SpeedController {
    bool isInitialized_ {false};
    int throttleLim_ {};
    MotorEsc esc_ {FlightConfig::ESC_PIN};

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


    void setThrottleLim(int percent) noexcept
    {
        assert(isInitialized_);
        if (percent >= MotorEsc::MIN_THROT && percent <= MotorEsc::MAX_THROT) {
            throttleLim_ = percent;

        } else {
            printf("Error: Illegal value for throttle limit.\n");
            // TODO: Replace this with exception handling eventually.
        }
    }

    [[nodiscard]] int getThrottleLim(void) noexcept
    {
        assert(isInitialized_);
        return throttleLim_;
    }

    void setSpeed(int percent) noexcept
    {
        assert(isInitialized_);
        // If percent is out of bounds, do nothing
        if (percent >= MotorEsc::MIN_THROT && percent <= throttleLim_) {
            esc_.setSpeed(percent);
        }
    }

    [[nodiscard]] int getSpeed(void) noexcept
    {
        assert(isInitialized_);
        return esc_.getSpeed();
    }
} // namespace SpeedController
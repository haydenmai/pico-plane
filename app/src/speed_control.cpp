/**
 * @file speed_control.cpp
 * @brief Controls the speed of the plane.
 * @author Benley Hsiang
 * @date Jul-16-2025
 */

#include "speed_control.h"

#include <cassert>
#include <stdio.h> // Remove later if not needed

namespace SpeedController {
    constexpr int ESC_PIN {18};

    static bool isInitialized_ {false};
    static bool throttleLim_ {};
    MotorEsc esc_ {ESC_PIN};

    void init(int throttleLim)
    {
        assert(!isInitialized_);

        setThrottleLim(throttleLim);
        isInitialized_ = true;
    }


    void cleanup()
    {
        assert(isInitialized_);
        isInitialized_ = false;
    }


    void setThrottleLim(int percent) noexcept
    {
        if (percent >= MotorEsc::MIN_THROT && percent <= MotorEsc::MAX_THROT) {
            throttleLim_ = percent;

        } else {
            printf("Error: Illegal value for throttle limit.\n");
            // TODO: Replace this with exception handling eventually.
        }
    }

    [[nodiscard]] const int getThrottleLim(void) noexcept { return throttleLim_; }

    void setSpeed(int percent) noexcept
    {
        // If percent is out of bounds, do nothing
        if (percent >= MotorEsc::MIN_THROT && percent <= throttleLim_) {
            esc_.setSpeed(percent);
        }
    }

    [[nodiscard]] const int getSpeed(void) noexcept
    {
        return esc_.getSpeed();
    }
} // namespace SpeedController
/**
 * @file speed_control.cpp
 * @brief Controls the speed of the plane.
 * @author Benley Hsiang
 * @date Apr-30-2026
 */

#include "speed_control.h"
#include "flight_config.h"

#include "pico/stdlib.h"

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

    void setSpeed(double percent) noexcept
    {
        assert(isInitialized_);
        // If percent is out of bounds, do nothing
        if (percent >= static_cast<double>(MotorEsc::MIN_THROT)
            && percent <= static_cast<double>(throttleLim_)) {
            esc_.setSpeed(percent);
        }
    }

    [[nodiscard]] int getSpeed(void) noexcept
    {
        assert(isInitialized_);
        return esc_.getSpeed();
    }

    void calibrate()
    {
        assert(isInitialized_);

        // Save current throttle limit and temporarily raise to full for calibration
        int prevLimit {throttleLim_};
        throttleLim_ = MotorEsc::MAX_THROT;

        // Typical ESC calibration: max throttle on power-up, then min throttle
        printf("ESC calibration: setting MAX throttle\n");
        esc_.setSpeed(static_cast<double>(MotorEsc::MAX_THROT));
        sleep_ms(3000);

        printf("ESC calibration: setting MIN throttle\n");
        esc_.setSpeed(static_cast<double>(MotorEsc::MIN_THROT));
        sleep_ms(1500);

        // Restore previous throttle limit and stop motor
        throttleLim_ = prevLimit;
        esc_.setSpeed(static_cast<double>(MotorEsc::MIN_THROT));
    }
} // namespace SpeedController
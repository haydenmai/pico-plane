/**
 * @file speed_control.h
 * @brief Controls the speed of the plane.
 * @author Benley Hsiang
 * @date Nov-14-2025
 */

#ifndef SPEED_CONTROL_H_
#define SPEED_CONTROL_H_

#include "hal/motor_esc.h"

/**
 * @namespace SpeedController
 */
namespace SpeedController {
    /**
     * @brief Initialization for the SpeedController module.
     * @param throttleLim Limit for how fast the motor spins as a percentage.
     *                    Percentage must be within [MIN_THROT, MAX_THROT].
     */
    void init();
    void cleanup();

    /**
     * @brief Sets the limit for how fast the motor spins.
     * @param percent The percentage of the motor's maximum throttle.
     * @pre Percentage must be within [MIN_THROT, MAX_THROT].
     */
    void setThrottleLim(int percent) noexcept;

    /**
     * @brief Retrieves the most recent value of the throttle limit.
     * @return Integer representing the throttle percentage limit.
     */
    [[nodiscard]] int getThrottleLim(void) noexcept;

    /**
     * @brief Sets how fast the the motor spins.
     * @param percent The percentage of the motor's maximum throttle.
     * @pre Percentage must be within [MIN_THROT, throttleLim_].
     * @note Values outside of [MIN_THROT, throttleLim_] are ignored.
     */
    void setSpeed(double percent) noexcept;

    /**
     * @brief Retrieves the value of the most recent setSpeed() call.
     * @return Integer representing the throttle percentage.
     */
    [[nodiscard]] int getSpeed(void) noexcept;

} // namespace SpeedController

#endif

/**
 * @file speed_control.h
 * @brief Controls the speed of the plane.
 * @author Benley Hsiang
 * @date Jul-09-2025
 */

#ifndef SPEED_CONTROL_H_
#define SPEED_CONTROL_H_

#include "hal/motor_esc.h"

/**
 * @class SpeedController
 */
class SpeedController {
  public:
    /**
     * @brief Constructor for the SpeedController class.
     * @param throttleLim Limit for how fast the motor spins as a percentage.
     *                    Percentage must be within [MIN_THROT, MAX_THROT].
     */
    explicit SpeedController(int throttleLim);
    ~SpeedController();

    /**
     * @brief Sets the limit for how fast the motor spins.
     * @param limit The percentage of the motor's maximum throttle.
     * @pre Percentage must be within [MIN_THROT, MAX_THROT].
     */
    void setThrottleLim(int limit) noexcept;

    /**
     * @brief Retrieves the most recent value of the throttle limit.
     * @return Integer representing the throttle percentage limit.
     */
    [[nodiscard]] int getThrottleLim(void) const noexcept;

    /**
     * @brief Sets how fast the the motor spins.
     * @param percent The percentage of the motor's maximum throttle.
     * @pre Percentage must be within [MIN_THROT, throttleLim_].
     * @note Values outside of [MIN_THROT, throttleLim_] are ignored.
     */
    void setSpeed(int percent) noexcept;

    /**
     * @brief Retrieves the value of the most recent setSpeed() call.
     * @return Integer representing the throttle percentage.
     */
    [[nodiscard]] int getSpeed(void) const noexcept;

  private:
    static constexpr int ESC_PIN = 18;
    MotorEsc esc_ {ESC_PIN};
    int throttleLim_ {}; ///< User-set limit for the throttle, never exceeds MAX_THROT
};

#endif

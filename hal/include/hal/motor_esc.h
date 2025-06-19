/**
 * @file motor_esc.h
 * @brief Manages the ESC (Electronic Speed Controller) for an electric motor.
 * @author Benley Hsiang
 * @date Jun-18-2025
 */

#ifndef MOTOR_ESC_H_
#define MOTOR_ESC_H_

#include <cstdint>

/**
 * @class MotorEsc
 * @brief Class for managing the ESC via PWM on a Raspberry Pi Pico W.
 */
class MotorEsc {
  public:
    /** @brief Minimum motor throttle as a percentage */
    static constexpr int MIN_THROT {0};

    /** @brief Maximum motor throttle as a percentage */
    static constexpr int MAX_THROT {100};

    explicit MotorEsc(int pinNum);
    ~MotorEsc();

    /**
     * @brief Sets how fast the the motor spins.
     * @param percent The percentage of the motor's maximum throttle.
     * @pre Percentage must be within [MIN_THROT, MAX_THROT].
     * @note Values outside of [MIN_THROT, MAX_THROT] are ignored.
     */
    void setSpeed(int percent) noexcept;

    /**
     * @brief Retrieves the value of the most recent setSpeed() call.
     * @return Integer representing the throttle percentage.
     */
    [[nodiscard]] int getSpeed() const noexcept;

  private:
    /** @brief PWM counter wrap value for a 10ms frame at 1MHz tick. */
    static constexpr int WRAP_COUNT {10000};
    /** @brief PWM clock divider to derive 1MHz from the 125MHz system clock. */
    static constexpr float DIVIDER {125.0f};

    const int pinNum_; ///< GPIO pin
    int sliceNum_;     ///< PWM slice index
    int channelNum_;   ///< PWM channel (A/B)
    int curSpeed_ {};  ///< Last speed percentage set

    /**
     * @brief Converts a throttle percentage to a pulse width in microseconds (us).
     * @param percent The percentage of the motor's maximum throttle.
     * @pre Percentage must be within [MIN_THROT, MAX_THROT].
     * @return Pulse width in microseconds (us) corresponding to the percentage.
     */
    [[nodiscard]] uint16_t percentToPulse_us(int percent) const noexcept;
};

#endif
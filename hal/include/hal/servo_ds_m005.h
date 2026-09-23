/**
 * @file servo_ds_m005.h
 * @brief Controls a DS-M005 servo via PWM on a Raspberry Pi Pico W.
 * @author Hayden Mai, Benley Hsiang
 * @date Jul-16-2025
 */

#ifndef SERVO_DS_M005_H_
#define SERVO_DS_M005_H_

#include <cstdint>

/**
 * @class ServoDSM005
 * @brief Class for driving a DS-M005 servo via PWM on a Raspberry Pi Pico W.
 */
class ServoDSM005 {
  public:
    /** @brief Minimum valid angle in degrees. */
    static constexpr int MIN_DEG {0};
    /** @brief Maximum valid angle in degrees. */
    static constexpr int MAX_DEG {180};

    /**
     * @brief Constructor for the ServoDSM005 class.
     * @param pinNum The GPIO pin on the Pico W connecting to the servo.
     * @pre Angles must be within [MIN_DEG, MAX_DEG].
     *      The lower limit must be less than the upper limit.
     */
    explicit ServoDSM005(int pinNum);
    ~ServoDSM005();

    /**
     * @brief Configures the PWM slice for a 1 µs tick at the current system clock.
     * @note Call after any system-clock change. Global constructors run at 125 MHz.
     */
    void configure() noexcept;

    /**
     * @brief Commands the servo to move to a specified angle.
     * @param degrees Target angle in degrees (0 to 180).
     * @pre Angle must be within [MIN_DEG, MAX_DEG].
     * @note Values outside [MIN_DEG, MAX_DEG] are ignored; no movement occurs.
     */
    void setAngle(int degrees) noexcept;

    /**
     * @brief Retrieves the value of the most recent setAngle() call.
     * @return Integer representing the angle in degrees.
     */
    [[nodiscard]] int getAngle() const noexcept;

  private:
    /** @brief PWM counter wrap value for a 20ms frame at 1MHz tick. */
    static constexpr int WRAP_COUNT {20000};

    const int pinNum_; ///< GPIO pin
    int sliceNum_;     ///< PWM slice index
    int channelNum_;   ///< PWM channel (A/B)
    int curAngle_ {};  ///< Last angle set by the user

    /**
     * @brief Converts an angle in degrees to a pulse width in microseconds.
     * @param angle Angle in degrees to convert.
     * @pre Angle must be within [MIN_DEG, MAX_DEG].
     * @return Pulse width in microseconds corresponding to the angle.
     */
    [[nodiscard]] uint16_t angleToPulse_us(int angle) const noexcept;
};

#endif

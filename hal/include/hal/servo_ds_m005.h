/**
 * @file servo_ds_m005.h
 * @brief Controls a DS-M005 servo via PWM on a Raspberry Pi Pico W.
 * @author Hayden Mai, Benley Hsiang
 * @date Jun-25-2025
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
     * @param degLowerLim The lower limit of the servo's range as an angle in degrees.
     * @param degUpperLim The upper limit of the servo's range as an angle in degrees.
     * @pre Angles must be within [MIN_DEG, MAX_DEG].
     *      The lower limit must be less than the upper limit.
     */
    explicit ServoDSM005(int pinNum, int degLowerLim, int degUpperLim);
    ~ServoDSM005();

    /**
     * @brief Sets the range of angles that the servo can spin within.
     * @param lower The lower limit of the servo's range as an angle in degrees.
     * @param upper The upper limit of the servo's range as an angle in degrees.
     * @pre Angles must be within [MIN_DEG, MAX_DEG].
     *      The lower limit must be less than the upper limit.
     */
    void setDegRange(int lower, int upper) noexcept;

    /**
     * @brief Retrieves the most recent lower limit value of the servo's range.
     * @return Integer representing the minimum angle in degrees.
     */
    [[nodiscard]] int getDegLowerLim(void) const noexcept;

    /**
     * @brief Retrieves the most recent upper limit value of the servo's range.
     * @return Integer representing the maximum angle in degrees.
     */
    [[nodiscard]] int getDegUpperLim(void) const noexcept;

    /**
     * @brief Commands the servo to move to a specified angle.
     * @param degrees Target angle in degrees (0 to 180).
     * @pre degrees must be within [MIN_DEG, MAX_DEG].
     * @note Values outside [MIN_DEG, MAX_DEG] are ignored; no movement occurs.
     */
    void setAngle(int degrees) noexcept;

    /**
     * @brief Retrieves the last commanded servo angle.
     * @return Last set angle in degrees.
     */
    [[nodiscard]] int getAngle() const noexcept;

  private:
    /** @brief PWM counter wrap value for a 20ms frame at 1MHz tick. */
    static constexpr int WRAP_COUNT {20000};
    /** @brief PWM clock divider to derive 1MHz from the 125MHz system clock. */
    static constexpr float DIVIDER {125.0f};

    const int pinNum_;   ///< GPIO pin
    int sliceNum_;       ///< PWM slice index
    int channelNum_;     ///< PWM channel (A/B)
    int curAngle_ {};    ///< Last angle set by the user
    int degLowerLim_ {}; ///< User-set lower limit for the servo's turning range.
    int degUpperLim_ {}; ///< User-set upper limit for the servo's turning range.

    /**
     * @brief Converts an angle in degrees to a pulse width in microseconds.
     * @param angle Angle in degrees to convert.
     * @pre angle must be within [MIN_DEG, MAX_DEG].
     * @return Pulse width in microseconds corresponding to the angle.
     */
    [[nodiscard]] uint16_t angleToPulse_us(int angle) const noexcept;
};

#endif

/**
 * @file servo_mg90s.h
 * @brief Controls an MG90S servo via PWM on a Pi Pico W.
 * @author Hayden Mai
 * @date May-01-2025
 */

#ifndef SERVO_MG90S_H_
#define SERVO_MG90S_H_

#include <cstdint>

// TODO: Only one instance per pin
// TODO?: Use exceptions?

/**
 * @class ServoMG90S
 * @brief Class for driving a MG90S servo on Raspberry Pi Pico W using PWM.
 */
class ServoMG90S {
  public:
    /** @brief Minimum valid angle in degrees. */
    static constexpr int MIN_DEG {0};
    /** @brief Maximum valid angle in degrees. */
    static constexpr int MAX_DEG {180};


    explicit ServoMG90S(int pinNum);
    ~ServoMG90S();

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

    const int pinNum; ///< GPIO pin
    int sliceNum;     ///< PWM slice index
    int channelNum;   ///< PWM channel (A/B)
    int cur_angle {}; ///< last commanded angle


    /**
     * @brief Converts an angle in degrees to a pulse width in microseconds.
     * @param angle Angle in degrees to convert.
     * @pre angle must be within [MIN_DEG, MAX_DEG].
     * @return Pulse width in microseconds corresponding to the angle.
     */
    [[nodiscard]] uint16_t angleToPulse_us(int angle) const noexcept;
};

#endif
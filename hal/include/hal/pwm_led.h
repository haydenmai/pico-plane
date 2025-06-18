/**
 * @file pwm_led.h
 * @brief Controls LED via PWM.
 * @author Hayden Mai
 * @date May-28-2025
 */

#ifndef PWM_LED_H_
#define PWM_LED_H_

#include <cstdint>

// TODO: Only one instance per pin
// TODO?: Use exceptions?

class pwmLED {
  public:

    explicit pwmLED(int pinNum);
    ~pwmLED();

    void setBrightness(int val) noexcept;

  private:
    /** @brief PWM counter wrap value for a 20ms frame at 1MHz tick. */
    static constexpr int WRAP_COUNT{10000};
    /** @brief PWM clock divider to derive 1MHz from the 125MHz system clock. */
    static constexpr float DIVIDER{125.0f};

    const int pinNum_; ///< GPIO pin
    int sliceNum_;     ///< PWM slice index
    int channelNum_;   ///< PWM channel (A/B)
    int cur_val_{};

    [[nodiscard]] uint16_t valToPulse_us(int val) const noexcept;
};

#endif
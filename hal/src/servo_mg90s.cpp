/**
 * @file servo_mg90s.cpp
 * @brief Controls an MG90S servo via PWM on a Pi Pico W.
 * @author Hayden Mai
 * @date May-01-2025
 */

#include "hal/servo_mg90s.h"

#include "hardware/pwm.h"
#include "pico/stdlib.h"


ServoMG90S::ServoMG90S(int pinNum) : pinNum(pinNum)
{
    // Route pin to the PWM block
    gpio_set_function(pinNum, GPIO_FUNC_PWM);
    sliceNum   = pwm_gpio_to_slice_num(pinNum);
    channelNum = pwm_gpio_to_channel(pinNum);

    pwm_config cfg = pwm_get_default_config();
    pwm_config_set_clkdiv(&cfg, DIVIDER);  // 125 MHz / 125 = 1MHz
    pwm_config_set_wrap(&cfg, WRAP_COUNT); // 20,000 ticks = 20ms (50hz)
    pwm_init(sliceNum, &cfg, true);
}


ServoMG90S::~ServoMG90S() { pwm_set_enabled(sliceNum, false); }


void ServoMG90S::setAngle(int degrees) noexcept
{
    // If degrees are out of bounds, do nothing
    if (degrees >= MIN_DEG && degrees <= MAX_DEG) {
        int pulse_us{angleToPulse_us(degrees)};
        pwm_set_chan_level(sliceNum, channelNum, pulse_us);
        cur_angle = degrees;
    }
}


[[nodiscard]] int ServoMG90S::getAngle() const noexcept { return cur_angle; }


[[nodiscard]] uint16_t ServoMG90S::angleToPulse_us(int angle) const noexcept
{
    /**
     * Let the servo's bearing be facing towards the viewer
     * 0 degrees 	-> .5ms pulse
     * 90 degrees	-> 1.5ms pulse
     * 180 degress	-> 2.5ms pulse
     */
    constexpr uint16_t MIN_US{500};              // Starting offset for pulse
    constexpr uint16_t DELTA_US{2000 / MAX_DEG}; // Per degree pulse difference

    return MIN_US + (angle * DELTA_US);
}
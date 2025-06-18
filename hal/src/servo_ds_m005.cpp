/**
 * @file servo_ds_m005.cpp
 * @brief Controls a DS-M005 servo via PWM on a Raspberry Pi Pico W.
 * @author Hayden Mai, Benley Hsiang
 * @date Jun-18-2025
 */

#include "hal/servo_ds_m005.h"

#include "hardware/pwm.h"
#include "pico/stdlib.h"

ServoDSM005::ServoDSM005(int pinNum) : pinNum(pinNum)
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

ServoDSM005::~ServoDSM005() { pwm_set_enabled(sliceNum, false); }

void ServoDSM005::setAngle(int degrees) noexcept
{
    // If degrees are out of bounds, do nothing
    if (degrees >= MIN_DEG && degrees <= MAX_DEG) {
        int pulse_us {angleToPulse_us(degrees)};
        pwm_set_chan_level(sliceNum, channelNum, pulse_us);
        curAngle = degrees;
    }
}

[[nodiscard]] int ServoDSM005::getAngle() const noexcept { return curAngle; }

[[nodiscard]] uint16_t ServoDSM005::angleToPulse_us(int angle) const noexcept
{
    /**
     * Let the servo's bearing be facing towards the viewer
     * 0 degrees 	->  500 us == 0.5 ms pulse
     * 90 degrees	-> 1500 us == 1.5 ms pulse
     * 180 degress	-> 2500 us == 2.5 ms pulse
     */
    constexpr uint16_t MIN_US {500}; // Starting offset for pulse
    constexpr uint16_t MAX_US {2500};

    // Per degree pulse difference
    constexpr uint16_t DELTA_US {(MAX_US - MIN_US) / MAX_DEG};

    return MIN_US + (angle * DELTA_US);
}
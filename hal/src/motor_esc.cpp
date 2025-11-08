/**
 * @file motor_esc.cpp
 * @brief Manages the ESC (Electronic Speed Controller) for an electric motor.
 * @author Benley Hsiang
 * @date Nov-07-2025
 */

#include "hal/motor_esc.h"

#include "hardware/pwm.h"
#include "pico/stdlib.h"

#include <stdio.h> // Remove later if not needed

MotorEsc::MotorEsc(int pinNum) : pinNum_(pinNum)
{
    // Route pin to the PWM block
    gpio_set_function(pinNum, GPIO_FUNC_PWM);
    sliceNum_   = pwm_gpio_to_slice_num(pinNum);
    channelNum_ = pwm_gpio_to_channel(pinNum);

    pwm_config cfg = pwm_get_default_config();
    pwm_config_set_clkdiv(&cfg, DIVIDER);  // 125 MHz / 125 = 1MHz
    pwm_config_set_wrap(&cfg, WRAP_COUNT); // 20,000 ticks = 20ms (50hz)
    pwm_init(sliceNum_, &cfg, true);
}

MotorEsc::~MotorEsc() { pwm_set_enabled(sliceNum_, false); }

void MotorEsc::setSpeed(int percent) noexcept
{
    // If percent is out of bounds, do nothing
    if (percent >= MIN_THROT && percent <= MAX_THROT) {
        int pulse_us {percentToPulse_us(percent)};
        pwm_set_chan_level(sliceNum_, channelNum_, pulse_us);
        curSpeed_ = percent;
    }
}

[[nodiscard]] int MotorEsc::getSpeed(void) const noexcept { return curSpeed_; }

[[nodiscard]] uint16_t MotorEsc::percentToPulse_us(int percent) const noexcept
{
    /**
     *   0% speed   -> 1000 us == 1.0 ms pulse
     *  50% speed   -> 1500 us == 1.5 ms pulse
     * 100% speed   -> 2000 us == 2.0 ms pulse
     */
    constexpr uint16_t MIN_US {1000};
    constexpr uint16_t MAX_US {2000};
    constexpr double PERCENT_SCALE {100.0};

    return static_cast<uint16_t>(
               static_cast<double>(percent)
               * ((static_cast<double>(MAX_US) - static_cast<double>(MIN_US)))
               / PERCENT_SCALE)
         + MIN_US;
}

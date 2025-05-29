/**
 * @file pwm_led.cpp
 * @brief Controls LED via PWM.
 * @author Hayden Mai
 * @date May-28-2025
 */

#include "hal/pwm_led.h"

#include "hardware/pwm.h"

pwmLED::pwmLED(int pinNum) : pinNum_(pinNum)
{
    // Route pin to the PWM block
    gpio_set_function(pinNum_, GPIO_FUNC_PWM);
    sliceNum_   = pwm_gpio_to_slice_num(pinNum_);
    channelNum_ = pwm_gpio_to_channel(pinNum_);

    pwm_config cfg = pwm_get_default_config();
    pwm_config_set_clkdiv(&cfg, DIVIDER);  // 125 MHz / 125 = 1MHz
    pwm_config_set_wrap(&cfg, WRAP_COUNT); // 20,000 ticks = 20ms (50hz)
    pwm_init(sliceNum_, &cfg, true);
}


pwmLED::~pwmLED() { pwm_set_enabled(sliceNum_, false); }


void pwmLED::setBrightness(int val) noexcept
{
    if (cur_val_ != val) {
        cur_val_ = val;
        int pulse_us {valToPulse_us(val)};
        pwm_set_chan_level(sliceNum_, channelNum_, pulse_us);
    }
}


[[nodiscard]] uint16_t pwmLED::valToPulse_us(int val) const noexcept
{
    val = val - 1000;
    return static_cast<double>(WRAP_COUNT) * static_cast<double>(val)
         / static_cast<double>(1000);
}
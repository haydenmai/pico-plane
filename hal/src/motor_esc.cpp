/**
 * @file motor_esc.h
 * @brief Manages the ESC (Electronic Speed Controller) for an electric motor.
 * @author Benley Hsiang
 * @date Jun-18-2025
 */

#include "hal/motor_esc.h"

#include "hardware/pwm.h"
#include "pico/stdlib.h"

MotorEsc::MotorEsc(int pinNum) : pinNum_(pinNum)
{
    // Route pin to the PWM block
    gpio_set_function(pinNum, GPIO_FUNC_PWM);
    sliceNum_   = pwm_gpio_to_slice_num(pinNum);
    channelNum_ = pwm_gpio_to_channel(pinNum);


    //
}
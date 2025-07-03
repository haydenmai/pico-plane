/**
 * @file pwm_led_control.cpp
 * @brief Manages the operation of the PWM LED(s).
 * @author Benley Hsiang
 * @date Jul-02-2025
 */

#include "pwm_led_control.h"

// GPIO pin numbers
static constexpr int PWM_LED_1_PIN = 17;
static constexpr int PWM_LED_2_PIN = 18;
static constexpr int PWM_LED_3_PIN = 19;
/**
 * @file pico_led.cpp
 * @brief Controls a single onboard LED via GPIO. Only one instance is allowed.
 *
 * This class initializes the onboard LED GPIO and allows it to be toggled on/off.
 * It enforces a single active instance to avoid multiple configurations of the same pin.
 *
 * @author Hayden Mai
 * @date Apr-21-2025
 */

#include "hal/pico_led.h"

#include "pico/cyw43_arch.h"
#include <cstdlib>


PicoLED &PicoLED::get() noexcept
{
    static PicoLED instance;
    return instance;
}


void PicoLED::set(bool on) noexcept
{
    // Ask the wifi "driver" to set the GPIO on or off
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, on);
}


void PicoLED::on() noexcept { set(true); }


void PicoLED::off() noexcept { set(false); }
/**
 * @file main.cpp
 *
 * @author Hayden Mai
 * @brief Controls an airplane and data
 * @date May-06-2025
 */


#include "hardware/adc.h"
#include "pico/cyw43_arch.h"
#include "pico/stdlib.h"

#include "hal/servo_mg90s.h"


int main()
{
    stdio_init_all();
    cyw43_arch_init();
    adc_init();

    auto servo_16 = ServoMG90S(16);
    auto servo_17 = ServoMG90S(17);


    while (true) {
        sleep_ms(1000);
    }

    stdio_deinit_all();
    cyw43_arch_deinit();
    return 0;
}

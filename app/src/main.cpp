/**
 * @file main.cpp
 *
 * @author Hayden Mai
 * @brief Controls an airplane and data
 * @date May-07-2025
 */

#include "hardware/adc.h"
#include "pico/cyw43_arch.h"
#include "pico/stdlib.h"

#include "hal/mpu6050.h"
#include "hal/pico_led.h"
#include "hal/servo_mg90s.h"

#include <cstdint>

int main()
{
    stdio_init_all();
    cyw43_arch_init();
    adc_init();

    auto &onboard_led = PicoLED::get();
    auto servo_16     = ServoMG90S(16);
    auto servo_17     = ServoMG90S(17);

    sleep_ms(10000);

    MPU6050 joe = MPU6050();

    while (true) {

        MPU6050::AccelVal joemama = joe.getAccelValues();
        MPU6050::GyroVal amogus   = joe.getGyroValues();

        printf("\nx: %.3fg, y: %.3fg, z: %.3fg\n", joemama.x, joemama.y, joemama.z);
        printf("\nx: %.3f deg/sec, y: %.3f deg/sec, z: %.3f deg/sec,\n", amogus.x,
               amogus.y, amogus.z);

        onboard_led.on();
        sleep_ms(50);
        onboard_led.off();
        sleep_ms(50);
    }

    stdio_deinit_all();
    cyw43_arch_deinit();

    return 0;
}

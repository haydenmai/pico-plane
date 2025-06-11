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

// I2C reserves some addresses for special purposes. We exclude these from the scan.
// These are any addresses of the form 000 0xxx or 111 1xxx
bool reserved_addr(uint8_t addr) { return (addr & 0x78) == 0 || (addr & 0x78) == 0x78; }

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

        printf("\n\nx: %.3fg\n", joemama.x);
        printf("y: %.3fg\n", joemama.y);
        printf("z: %.3fg\n", joemama.z);

        printf("\nx: %d\n", amogus.x);
        printf("y: %d\n", amogus.y);
        printf("z: %d\n\n", amogus.z);

        onboard_led.on();
        sleep_ms(2000);
        onboard_led.off();
        sleep_ms(5000);
    }

    stdio_deinit_all();
    cyw43_arch_deinit();

    return 0;
}

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

#include "hardware/i2c.h"
#include "pico/binary_info.h"

#include "hal/mpu6050.h"
#include "hal/pico_led.h"
#include "hal/servo_mg90s.h"

#include <cstdint>

#define ADDRESS_SIZE 7

#define MPU6050_ADDR 0x68

// Power Management Registers
#define PWR_MANAGE_1 0x6B
#define PWR_MANAGE_2 0x6C

// Configuration Registers
#define GYRO_CONFIG  0x1B
#define ACCEL_CONFIG 0x1C

// Slave 0 Control Registers
#define I2C_SLV0_ADDR 0x25
#define I2C_SLV0_REG  0x26
#define I2C_SLV0_CTRL 0x27

// Accelerometer Measurement Registers
#define ACCEL_X_HIGH 0x3B
#define ACCEL_X_LOW  0x3C
#define ACCEL_Y_HIGH 0x3D
#define ACCEL_Y_LOW  0x3E
#define ACCEL_Z_HIGH 0x3F
#define ACCEL_Z_LOW  0x40

// Gyroscope Measurement Registers
#define GYRO_X_HIGH 0x43
#define GYRO_X_LOW  0x44
#define GYRO_Y_HIGH 0x45
#define GYRO_Y_LOW  0x46
#define GYRO_Z_HIGH 0x47
#define GYRO_Z_LOW  0x48


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

#if !defined(i2c_default) || !defined(PICO_DEFAULT_I2C_SDA_PIN)                          \
    || !defined(PICO_DEFAULT_I2C_SCL_PIN)
#warning i2c/bus_scan example requires a board with I2C pins
    puts("Default I2C pins were not defined");
#else
    // This example will use I2C0 on the default SDA and SCL pins (GP4, GP5 on a Pico)
    i2c_init(i2c_default, 100 * 1000);
    gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);
    gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);
    // Make the I2C pins available to picotool
    bi_decl(bi_2pins_with_func(PICO_DEFAULT_I2C_SDA_PIN, PICO_DEFAULT_I2C_SCL_PIN,
                               GPIO_FUNC_I2C));


    while (true) {
        uint8_t reg = ACCEL_X_HIGH;

        // uint8_t data[2] = {PWR_MANAGE_1, 0x00};
        i2c_write_blocking(i2c_default, MPU6050_ADDR, &reg, 1, true);

        uint8_t readings[6];
        int val = i2c_read_blocking(i2c_default, MPU6050_ADDR, readings, 6, false);
        if (val == 0) {
        }

        printf("\n\nReadings:\n");
        for (int i = 0; i < 6; i++) {
            printf("readings[%d]: %#X\n", i, readings[i]);
        }
        /*
        printf("\nI2C Bus Scan\n");
        printf("   0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F\n");

        for (int addr = 0; addr < (1 << 7); ++addr) {
            if (addr % 16 == 0) {
                printf("%02x ", addr);
            }

            // Perform a 1-byte dummy read from the probe address. If a slave
            // acknowledges this address, the function returns the number of bytes
            // transferred. If the address byte is ignored, the function returns
            // -1.

            // Skip over any reserved addresses.
            int ret;
            uint8_t rxdata;
            if (reserved_addr(addr))
                ret = PICO_ERROR_GENERIC;
            else
                ret = i2c_read_blocking(i2c_default, addr, &rxdata, 1, false);

            printf(ret < 0 ? "." : "@");
            printf(addr % 16 == 15 ? "\n" : "  ");
        }
        printf("Done.\n");
        */

        onboard_led.on();
        sleep_ms(2000);
        onboard_led.off();
        sleep_ms(5000);
    }

    stdio_deinit_all();
    cyw43_arch_deinit();
    return 0;

#endif
}

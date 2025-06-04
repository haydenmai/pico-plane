/**
 * @file mpu6050.cpp
 * @brief Controls the MPU 6050 accelerometer and gyroscope.
 * @author Benley Hsiang
 * @date Jun-04-2025
 */

#include "hal/mpu6050.h"

#include "hardware/i2c.h"
#include "pico/binary_info.h"
#include "pico/stdlib.h"

#define ADDRESS_SIZE 7

// Power Management Registers
#define PWR_MANAGE_1 0x6B
#define PWR_MANAGE_2 0x6C

// Configuration Registers
#define CONFIG       0x1A
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

/**
 *
 *
 * Taken from:
 * https://www.raspberrypi.com/documentation/pico-sdk/hardware.html#group_hardware_i2c
 *
 * Note: I have a feeling this probably doesn't work for multiple instances of the object
 */
MPU6050::MPU6050()
{
#if !defined(i2c_default) || !defined(PICO_DEFAULT_I2C_SDA_PIN)                          \
    || !defined(PICO_DEFAULT_I2C_SCL_PIN)
#warning A board with I2C pins is required
    puts("Default I2C pins were not defined");
#else
    // Using I2C0 on the default SDA and SCL pins (GP4, GP5 on a Pico)
    i2c_init(i2c_default, 100 * 1000);
    gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);
    gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);
    // Make the I2C pins available to picotool
    bi_decl(bi_2pins_with_func(PICO_DEFAULT_I2C_SDA_PIN, PICO_DEFAULT_I2C_SCL_PIN,
                               GPIO_FUNC_I2C));
#endif
}

MPU6050::~MPU6050() { i2c_deinit(i2c_default); }

[[nodiscard]] MPU6050::GyroVal MPU6050::getGyroValues()
{
    readGyroValues();

    return gyroVals_;
}

[[nodiscard]] MPU6050::AccelVal MPU6050::getAccelValues()
{
    readAccelValues();

    return accelVals_;
}

void readGyroValues()
{
    // TODO
}

void readAccelValues()
{
    // TODO
}

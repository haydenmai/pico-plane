/**
 * @file mpu6050.cpp
 * @brief Controls the MPU 6050 accelerometer and gyroscope.
 * @author Benley Hsiang
 * @date Jun-10-2025
 */

#include "hal/mpu6050.h"

#include "hardware/i2c.h"
#include "pico/binary_info.h"
#include "pico/stdlib.h"

#include <cstdint>

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


// I2C Writing and Reading
#define MPU6050_ADDR  0x68
#define NUM_REGISTERS 6

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

// Full Scale Ranges
#define ACCEL_RANGE    /* +- */ 2   /* g */
#define GYRO_RANGE     /* +- */ 250 /* degrees per sec */
#define BIT_RESOLUTION 16

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

void MPU6050::readGyroValues()
{
    uint8_t reg = GYRO_X_HIGH;
    i2c_write_blocking(i2c_default, MPU6050_ADDR, &reg, 1, true);

    uint8_t readings[NUM_REGISTERS];
    i2c_read_blocking(i2c_default, MPU6050_ADDR, readings, NUM_REGISTERS, false);

    // Combining high and low bits
    const int BIT_OFFSET = 8;
    int16_t gyro_raw_x   = ((int16_t)readings[0] << BIT_OFFSET) | readings[1];
    int16_t gyro_raw_y   = ((int16_t)readings[2] << BIT_OFFSET) | readings[3];
    int16_t gyro_raw_z   = ((int16_t)readings[4] << BIT_OFFSET) | readings[5];

    // TODO: Figure out if the readings need to be converted

    this->gyroVals_.x = gyro_raw_x;
    this->gyroVals_.y = gyro_raw_y;
    this->gyroVals_.z = gyro_raw_z;
}

void MPU6050::readAccelValues()
{
    uint8_t reg = ACCEL_X_HIGH;
    i2c_write_blocking(i2c_default, MPU6050_ADDR, &reg, 1, true);

    uint8_t readings[NUM_REGISTERS];
    i2c_read_blocking(i2c_default, MPU6050_ADDR, readings, NUM_REGISTERS, false);

    // Combining high and low bits
    const int BIT_OFFSET = 8;
    int16_t accel_raw_x  = ((int16_t)readings[0] << BIT_OFFSET) | readings[1];
    int16_t accel_raw_y  = ((int16_t)readings[2] << BIT_OFFSET) | readings[3];
    int16_t accel_raw_z  = ((int16_t)readings[4] << BIT_OFFSET) | readings[5];

    double g_value_x = MPU6050::convertAccelReadings(accel_raw_x);
    double g_value_y = MPU6050::convertAccelReadings(accel_raw_y);
    double g_value_z = MPU6050::convertAccelReadings(accel_raw_z);

    this->accelVals_.x = g_value_x;
    this->accelVals_.y = g_value_y;
    this->accelVals_.z = g_value_z;
}

double MPU6050::convertAccelReadings(int16_t reading)
{
    double g_value
        = reading * ((double)(ACCEL_RANGE * 2) / (double)(1 << BIT_RESOLUTION));

    return g_value;
}

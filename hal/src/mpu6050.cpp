/**
 * @file mpu6050.cpp
 * @brief Controls the MPU 6050 accelerometer and gyroscope.
 * @author Benley Hsiang
 * @date Jun-11-2025
 */

#include "hal/mpu6050.h"

#include "hardware/i2c.h"
#include "pico/binary_info.h"
#include "pico/stdlib.h"

#include <cstdint>

/**
 * Taken from:
 * https://www.raspberrypi.com/documentation/pico-sdk/hardware.html#group_hardware_i2c
 *
 * Note: I have a feeling this probably doesn't work for multiple instances of the
 * object
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

[[nodiscard]] MPU6050::AccelVal MPU6050::getAccelValues(void)
{
    readAccelValues();

    return accelVals_;
}

[[nodiscard]] MPU6050::GyroVal MPU6050::getGyroValues(void)
{
    readGyroValues();

    return gyroVals_;
}

void MPU6050::readAccelValues(void)
{
    uint8_t reg = ACCEL_X_HIGH;
    i2c_write_blocking(i2c_default, MPU6050_ADDR, &reg, 1, true);

    uint8_t readings[NUM_REGISTERS];
    i2c_read_blocking(i2c_default, MPU6050_ADDR, readings, NUM_REGISTERS, false);

    const int BIT_OFFSET = 8;
    int16_t accel_raw_x  = combineBits(readings, 0, 1, BIT_OFFSET);
    int16_t accel_raw_y  = combineBits(readings, 2, 3, BIT_OFFSET);
    int16_t accel_raw_z  = combineBits(readings, 4, 5, BIT_OFFSET);

    accelVals_.x = convertAccelReading(accel_raw_x, ACCEL_RANGE) + 0.012; // Calibrating
    accelVals_.y = convertAccelReading(accel_raw_y, ACCEL_RANGE) - 0.005; // the values
    accelVals_.z = convertAccelReading(accel_raw_z, ACCEL_RANGE) - 0.12; // so 0/1 at rest
}

void MPU6050::readGyroValues(void)
{
    uint8_t reg = GYRO_X_HIGH;
    i2c_write_blocking(i2c_default, MPU6050_ADDR, &reg, 1, true);

    uint8_t readings[NUM_REGISTERS];
    i2c_read_blocking(i2c_default, MPU6050_ADDR, readings, NUM_REGISTERS, false);

    const int BIT_OFFSET = 8;
    int16_t gyro_raw_x   = combineBits(readings, 0, 1, BIT_OFFSET);
    int16_t gyro_raw_y   = combineBits(readings, 2, 3, BIT_OFFSET);
    int16_t gyro_raw_z   = combineBits(readings, 4, 5, BIT_OFFSET);

    gyroVals_.x = convertGyroReading(gyro_raw_x, GYRO_SENS) - 0.5; // Calibrating the
    gyroVals_.y = convertGyroReading(gyro_raw_y, GYRO_SENS) - 4.1; // values so they're
    gyroVals_.z = convertGyroReading(gyro_raw_z, GYRO_SENS) + 0.2; // 0 at rest
}

int16_t MPU6050::combineBits(uint8_t bitsArr[], int highBits, int lowBits,
                             const int offset)
{
    int16_t combined = ((int16_t)bitsArr[highBits] << offset) | bitsArr[lowBits];

    return combined;
}

double MPU6050::convertAccelReading(int16_t reading, double range)
{
    double g_val = reading * ((range * 2) / (double)(1 << BIT_RESOLUTION));

    return g_val;
}

double MPU6050::convertGyroReading(int16_t reading, double lsb_sens)
{
    return (double)reading / lsb_sens;
}

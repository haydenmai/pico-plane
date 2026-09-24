/**
 * @file mpu6050.cpp
 * @brief Controls the MPU 6050 accelerometer and gyroscope.
 * @author Benley Hsiang
 * @date Sep-22-2026
 */

#include "hal/mpu6050.h"

#include "hardware/i2c.h"
#include "pico/binary_info.h"
#include "pico/stdlib.h"

#include <cstdint>

namespace {
    // I2C0 on GP8/GP9. Default I2C pins (GP4, GP5) are used by UART1.
    constexpr uint I2C_SDA_PIN {8};
    constexpr uint I2C_SCL_PIN {9};
    i2c_inst_t *const I2C_PORT {i2c0};
    // Keep shorter than the 5 ms filter period so a missing IMU cannot stall
    // the core that writes motor/servo commands.
    constexpr uint I2C_TIMEOUT_US {2000};
} // namespace

/**
 * Taken from:
 * https://www.raspberrypi.com/documentation/pico-sdk/hardware.html#group_hardware_i2c
 *
 * Note: I have a feeling this probably doesn't work for multiple instances of the
 * object
 */
MPU6050::MPU6050()
{
    i2c_init(I2C_PORT, 100 * 1000);
    gpio_set_function(I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA_PIN);
    gpio_pull_up(I2C_SCL_PIN);
    // Make the I2C pins available to picotool
    bi_decl(bi_2pins_with_func(I2C_SDA_PIN, I2C_SCL_PIN, GPIO_FUNC_I2C));

    // MPU6050 powers up in sleep. A timed write avoids hanging if the chip
    // is missing or the bus is stuck.
    uint8_t wake_cmd[] {PWR_MANAGE_1, 0x00};
    i2c_write_timeout_us(I2C_PORT, MPU6050_ADDR, wake_cmd, 2, false, I2C_TIMEOUT_US);
}

MPU6050::~MPU6050() { i2c_deinit(I2C_PORT); }

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
    if (i2c_write_timeout_us(I2C_PORT, MPU6050_ADDR, &reg, 1, true, I2C_TIMEOUT_US)
        != 1) {
        return;
    }

    uint8_t readings[NUM_REGISTERS];
    if (i2c_read_timeout_us(I2C_PORT, MPU6050_ADDR, readings, NUM_REGISTERS, false,
                            I2C_TIMEOUT_US)
        != NUM_REGISTERS) {
        return;
    }

    const int BIT_OFFSET = 8;
    int16_t accel_raw_x  = combineBits(readings, 0, 1, BIT_OFFSET);
    int16_t accel_raw_y  = combineBits(readings, 2, 3, BIT_OFFSET);
    int16_t accel_raw_z  = combineBits(readings, 4, 5, BIT_OFFSET);

    accelVals_.x = convertAccelReading(accel_raw_x, ACCEL_RANGE);
    accelVals_.y = convertAccelReading(accel_raw_y, ACCEL_RANGE);
    accelVals_.z = convertAccelReading(accel_raw_z, ACCEL_RANGE);
}

void MPU6050::readGyroValues(void)
{
    uint8_t reg = GYRO_X_HIGH;
    if (i2c_write_timeout_us(I2C_PORT, MPU6050_ADDR, &reg, 1, true, I2C_TIMEOUT_US)
        != 1) {
        return;
    }

    uint8_t readings[NUM_REGISTERS];
    if (i2c_read_timeout_us(I2C_PORT, MPU6050_ADDR, readings, NUM_REGISTERS, false,
                            I2C_TIMEOUT_US)
        != NUM_REGISTERS) {
        return;
    }

    const int BIT_OFFSET = 8;
    int16_t gyro_raw_x   = combineBits(readings, 0, 1, BIT_OFFSET);
    int16_t gyro_raw_y   = combineBits(readings, 2, 3, BIT_OFFSET);
    int16_t gyro_raw_z   = combineBits(readings, 4, 5, BIT_OFFSET);

    gyroVals_.x = convertGyroReading(gyro_raw_x, GYRO_SENS);
    gyroVals_.y = convertGyroReading(gyro_raw_y, GYRO_SENS);
    gyroVals_.z = convertGyroReading(gyro_raw_z, GYRO_SENS);
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

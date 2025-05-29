/**
 * @file mpu6050.cpp
 * @brief Controls the MPU 6050 accelerometer and gyroscope.
 * @author Benley Hsiang
 * @date May-28-2025
 */

#include "hal/mpu6050.h"

#include "hardware/i2c.h"
#include "pico/binary_info.h"
#include "pico/stdlib.h"


MPU6050::MPU6050()
{
    // TODO: Initialize i2c
}

MPU6050::~MPU6050()
{
    // TODO: Deinitialize i2c
}

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

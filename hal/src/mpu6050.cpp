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

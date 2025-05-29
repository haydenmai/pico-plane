/**
 * @file mpu6050.h
 * @brief Controls the MPU 6050 accelerometer and gyroscope.
 * @author Benley Hsiang
 * @date May-28-2025
 */

#ifndef MPU_6050_H_
#define MPU_6050_H_

#include <cstdint>

/**
 * @class MPU6050
 * @brief
 */
class MPU6050 {
    // Note to self: Private members' names will have an underscore at the end,
    // public ones have a double underscore at the end
  public:
    MPU6050();
    ~MPU6050();

    /**
     * @struct GyroVal
     * @brief Holds gyroscope readings for the x, y, and z axes.
     */
    typedef struct {
        uint16_t x;
        uint16_t y;
        uint16_t z;
    } GyroVal;

    /**
     * @struct AccelVal
     * @brief Holds accelerometer readings for the x, y, and z directions.
     */
    typedef struct {
        uint16_t x;
        uint16_t y;
        uint16_t z;
    } AccelVal;

    /**
     * @brief Samples the gyroscope values.
     * @return GyroVal struct containing XYZ readings from the gyroscope.
     */
    [[nodiscard]] GyroVal getGyroValues();

    /**
     * @brief Samples the accelerometer values.
     * @return AccelVal struct containing XYZ readings from the accelerometer.
     */
    [[nodiscard]] AccelVal getAccelValues();

  private:
    GyroVal gyroVal_;
    AccelVal accelVal_;
};

#endif
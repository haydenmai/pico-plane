/**
 * @file mpu6050.h
 * @brief Controls the MPU 6050 accelerometer and gyroscope.
 * @author Benley Hsiang
 * @date Jun-10-2025
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
        int16_t x; // Note: This will probably not stay as int16_t, revisit this
        int16_t y;
        int16_t z;
    } GyroVal;

    /**
     * @struct AccelVal
     * @brief Holds accelerometer readings for the x, y, and z directions.
     */
    typedef struct {
        double x;
        double y;
        double z;
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
    GyroVal gyroVals_;
    AccelVal accelVals_;

    /**
     *
     */
    void readGyroValues();

    /**
     *
     */
    void readAccelValues();

    /**
     *
     */
    double convertAccelReadings(int16_t reading);

    /**
     *
     */
    // double convertGyroReadings(int16_t reading);
};

#endif
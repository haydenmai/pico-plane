/**
 * @file mpu6050.h
 * @brief Controls the MPU 6050 accelerometer and gyroscope.
 * @author Benley Hsiang
 * @date Jun-11-2025
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
        double x;
        double y;
        double z;
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
    int16_t combineBits(uint8_t bitsArr[], int highBits, int lowBits, const int offset);

    /**
     *
     */
    double convertAccelReading(int16_t reading, int range);

    /**
     *
     */
    double convertGyroReading(int16_t reading, double lsb_sens);
};

#endif
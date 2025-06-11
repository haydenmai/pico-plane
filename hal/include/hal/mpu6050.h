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
     * @struct AccelVal
     * @brief Holds accelerometer readings for the x, y, and z directions.
     */
    typedef struct {
        double x;
        double y;
        double z;
    } AccelVal;

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
     * @brief Samples the accelerometer values.
     * @return AccelVal struct containing XYZ readings from the accelerometer.
     */
    [[nodiscard]] AccelVal getAccelValues();

    /**
     * @brief Samples the gyroscope values.
     * @return GyroVal struct containing XYZ readings from the gyroscope.
     */
    [[nodiscard]] GyroVal getGyroValues();

  private:
    AccelVal accelVals_;
    GyroVal gyroVals_;

    // Power Management Registers
    static constexpr uint8_t PWR_MANAGE_1 = 0x6B;
    static constexpr uint8_t PWR_MANAGE_2 = 0x6C;

    // Configuration Registers
    static constexpr uint8_t GYRO_CONFIG  = 0x1B;
    static constexpr uint8_t ACCEL_CONFIG = 0x1C;

    // Slave 0 Control Registers
    static constexpr uint8_t I2C_SLV0_ADDR = 0x25;
    static constexpr uint8_t I2C_SLV0_REG  = 0x26;
    static constexpr uint8_t I2C_SLV0_CTRL = 0x27;

    // I2C Writing and Reading
    static constexpr uint8_t MPU6050_ADDR = 0x68;
    static constexpr int NUM_REGISTERS    = 6;
    static constexpr int BIT_RESOLUTION   = 16;

    // Measurement Registers
    static constexpr uint8_t ACCEL_X_HIGH = 0x3B;
    static constexpr uint8_t GYRO_X_HIGH  = 0x43;

    // Full Scale Range/LSB Sensitivity
    static constexpr double ACCEL_RANGE = /* +- */ 2.0 /* g */;
    static constexpr double GYRO_SENS   = 131.0 /* LSB/ degree/second */;

    /**
     *
     */
    void readAccelValues();

    /**
     *
     */
    void readGyroValues();

    /**
     *
     */
    int16_t combineBits(uint8_t bitsArr[], int highBits, int lowBits, const int offset);

    /**
     *
     */
    double convertAccelReading(int16_t reading, double range);

    /**
     *
     */
    double convertGyroReading(int16_t reading, double lsb_sens);
};

#endif
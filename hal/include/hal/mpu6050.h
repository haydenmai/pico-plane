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
 * @brief Class for sampling and storing the readings taken from the MPU 6050
 * accelerometer and gyroscope.
 */
class MPU6050 {
  public:
    MPU6050();
    ~MPU6050();

    /**
     * @struct AccelVal
     * @brief Holds accelerometer readings for the x, y, and z axes.
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
    [[nodiscard]] AccelVal getAccelValues(void);

    /**
     * @brief Samples the gyroscope values.
     * @return GyroVal struct containing XYZ readings from the gyroscope.
     */
    [[nodiscard]] GyroVal getGyroValues(void);

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
     * @brief Reads from the accelerometer and stores the values in accelVals_ private
     * data member.
     */
    void readAccelValues(void);

    /**
     * @brief Reads from the gyroscope and stores the values in the gyroVals_ private
     * data member.
     */
    void readGyroValues(void);

    /**
     * @brief Combines the high and low bits into a single int16_t value.
     *
     * @param bitsArr   Array containing uint8_t values, i.e. the high and low bits.
     * @param highBits  Index of the high bits.
     * @param lowBits   Index of the low bits.
     * @param offset    How much the high bits are shifted to the left.
     *
     * @return Combined bits as an int16_t.
     */
    int16_t combineBits(uint8_t bitsArr[], int highBits, int lowBits, const int offset);

    /**
     * @brief Takes the raw hex value of the accelerometer and converts it to a readable
     * acceleration value in g's.
     * @return Accelerometer reading as a double, in g's.
     */
    double convertAccelReading(int16_t reading, double range);

    /**
     * @brief Takes the raw hex value of the gyroscope and converts it to a readable
     * angular velocity value in degrees per second.
     * @return Gyroscope reading as a double, in degrees per second.
     */
    double convertGyroReading(int16_t reading, double lsb_sens);
};

#endif
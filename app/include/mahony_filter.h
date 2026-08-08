/**
 * @file mahony_filter.h
 * @brief Filters the raw values from the MPU6050 accelerometer and gyroscope.
 * @author Benley Hsiang
 * @date Aug-04-2026
 */

#ifndef MAHONY_FILTER_H_
#define MAHONY_FILTER_H_

#include "hal/mpu6050.h"

/**
 * @namespace MahonyFilter
 * @brief Filters raw accelerometer and gyroscope values.
 */
namespace MahonyFilter {

    /**
     * @brief Sets the filter's proportional gain, i.e. how much the filter uses the
     * accelerometer values to correct for the gyroscope's immediate error.
     * @note Typically ranges between 0.5 and 5.0, the default is currently 1.0.
     *
     * Low Kp values can result in slower correction of gyroscope drift.
     *
     * High Kp values can result in overcorrecting the gyroscope values during sharp
     * turns.
     */
    void setKp(float Kp);

    /**
     * @brief Sets the filter's integral gain, i.e. how much the filter corrects for the
     * gyroscope's accumulated error.
     * @note Typically ranges between 0.0 and 0.1, the default is currently 0.0.
     *
     * Low Ki values can result in good response to sharp turns, but suffers more from
     * long-term gyroscope drift.
     *
     * High Ki values can eliminate gyroscope drift very quickly but can also fight
     * against actual sustained rotation.
     */
    void setKi(float Ki);

    /**
     * @brief Updates the quaternion (a 4D vector with a length of always 1.0) which
     * represents and tracks the current 3D orientation. It takes the raw sensor data and
     * the time since the last update, and uses the Mahony fusion algorithm to update the
     * quaternion.
     *
     * @param ax The accelerometer's raw x value in g (gravity units).
     * @param ay The accelerometer's raw y value in g (gravity units).
     * @param az The accelerometer's raw z value in g (gravity units).
     *
     * @param gx The gyroscope's raw x value in radians per second.
     * @param gy The gyroscope's raw y value in radians per second.
     * @param gz The gyroscope's raw z value in radians per second.
     *
     * @param dt Seconds since the last update.
     */
    void update(float ax, float ay, float az, float gx, float gy, float gz, float dt);

    /**
     * @brief Returns the current roll angle, i.e. the rotation around the forward axis.
     * @return Roll angle in degrees.
     */
    [[nodiscard]] float getRoll(void) noexcept;

    /**
     * @brief Returns the current pitch angle, i.e. the rotation around the lateral axis.
     * @return Pitch angle in degrees.
     */
    [[nodiscard]] float getPitch(void) noexcept;

    /**
     * @brief Returns the current yaw angle, i.e. the rotation around the vertical axis.
     * @return Yaw angle in degrees.
     */
    [[nodiscard]] float getYaw(void) noexcept;

    /**
     * @brief Resets the filter to its default values. For use in case of emergency.
     */
    void reset(void);

} // namespace MahonyFilter

#endif

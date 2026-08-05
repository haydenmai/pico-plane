/**
 * @file mahony_filter.h
 * @brief Filters the raw values from the MPU6050 accelerometer and gyroscope.
 * @author Benley Hsiang
 * @date Aug-04-2026
 */

#ifndef MAHONY_FILTER_H_
#define MAHONY_FILTER_H_

#include "hal/mpu6050.h"

namespace MahonyFilter {

    /**
     * @brief Sets the confidence level of the raw readings.
     */
    void setKp(float Kp);

    /**
     * @brief Sets the error between the attitude calculation result and the true
     * attitude.
     */
    void setKi(float Ki);

    /**
     * @brief
     */
    void update(); // TODO

    /**
     * @brief
     * @return
     */
    [[nodiscard]] float getRoll(void) noexcept;

    /**
     * @brief
     * @return
     */
    [[nodiscard]] float getPitch(void) noexcept;

    /**
     * @brief
     * @return
     */
    [[nodiscard]] float getYaw(void) noexcept;

} // namespace MahonyFilter

#endif

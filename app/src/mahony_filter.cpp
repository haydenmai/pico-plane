/**
 * @file mahony_filter.cpp
 * @brief Filters the raw values from the MPU6050 accelerometer and gyroscope.
 * @author Benley Hsiang
 * @date Aug-08-2026
 */

#include "mahony_filter.h"
#include <cmath>

void MahonyFilter::setKp(float Kp) { kp_ = Kp; }

void MahonyFilter::setKi(float Ki) { ki_ = Ki; }

void MahonyFilter::update(float ax, float ay, float az, float gx, float gy, float gz,
                          float dt)
{
    // Normalizing accelerometer vector
    float accel_norm = sqrtf(ax * ax + ay * ay + az * az);
    if (accel_norm == 0.0f) {
        return;
    }
    ax /= accel_norm;
    ay /= accel_norm;
    az /= accel_norm;

    // Computing the error
    float vx      = 2.0f * (q1_ * q3_ - q0_ * q2_);
    float vy      = 2.0f * (q0_ * q1_ + q2_ * q3_);
    float vz      = (q0_ * q0_) - (q1_ * q1_) - (q2_ * q2_) + (q3_ * q3_);
    float error_x = ay * vz - az * vy;
    float error_y = az * vx - ax * vz;
    float error_z = ax * vy - ay * vx;

    // Applying integral feedback
    if (ki_ > 0.0f) {
        integralFBx_ += ki_ * error_x * dt;
        integralFBy_ += ki_ * error_y * dt;
        integralFBz_ += ki_ * error_z * dt;
    } else {
        integralFBx_ = 0.0f;
        integralFBy_ = 0.0f;
        integralFBz_ = 0.0f;
    }

    // Applying Kp
    gx += kp_ * error_x + integralFBx_;
    gy += kp_ * error_y + integralFBy_;
    gz += kp_ * error_z + integralFBz_;

    // Integrating
    float half_dt = 0.5f * dt;
    float gx_half = gx * half_dt;
    float gy_half = gy * half_dt;
    float gz_half = gz * half_dt;

    float q0_new = q0_ - (q1_ * gx_half) - (q2_ * gy_half) - (q3_ * gz_half);
    float q1_new = q1_ + (q0_ * gx_half) - (q3_ * gy_half) + (q2_ * gz_half);
    float q2_new = q2_ + (q3_ * gx_half) + (q0_ * gy_half) - (q1_ * gz_half);
    float q3_new = q3_ - (q2_ * gx_half) + (q1_ * gy_half) + (q0_ * gz_half);

    q0_ = q0_new;
    q1_ = q1_new;
    q2_ = q2_new;
    q3_ = q3_new;

    // Normalizing quaternion
    float quat_norm = sqrtf(q0_ * q0_ + q1_ * q1_ + q2_ * q2_ + q3_ * q3_);
    if (quat_norm == 0.0f) {
        return;
    }
    q0_ /= quat_norm;
    q1_ /= quat_norm;
    q2_ /= quat_norm;
    q3_ /= quat_norm;

    computeEulerAngles();
}

[[nodiscard]] float MahonyFilter::getRoll(void) const noexcept { return roll_; }

[[nodiscard]] float MahonyFilter::getPitch(void) const noexcept { return pitch_; }

[[nodiscard]] float MahonyFilter::getYaw(void) const noexcept { return yaw_; }

void MahonyFilter::reset(void)
{
    q0_          = 1.0f;
    q1_          = 0.0f;
    q2_          = 0.0f;
    q3_          = 0.0f;
    integralFBx_ = 0.0f;
    integralFBy_ = 0.0f;
    integralFBz_ = 0.0f;
    roll_        = 0.0f;
    pitch_       = 0.0f;
    yaw_         = 0.0f;
}

void MahonyFilter::computeEulerAngles(void)
{
    roll_ = atan2f(2.0f * (q0_ * q1_ + q2_ * q3_), 1.0f - 2.0f * (q1_ * q1_ + q2_ * q2_))
          * RAD_TO_DEG;

    pitch_ = asinf(2.0f * (q0_ * q2_ - q3_ * q1_)) * RAD_TO_DEG;

    yaw_ = atan2f(2.0f * (q0_ * q3_ + q1_ * q2_), 1.0f - 2.0f * (q2_ * q2_ + q3_ * q3_))
         * RAD_TO_DEG;
}

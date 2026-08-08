/**
 * @file mahony_filter.h
 * @brief Filters the raw values from the MPU6050 accelerometer and gyroscope.
 * @author Benley Hsiang
 * @date Aug-08-2026
 */

#ifndef MAHONY_FILTER_H_
#define MAHONY_FILTER_H_

/**
 * @class MahonyFilter
 * @brief Filters raw accelerometer and gyroscope values.
 */
class MahonyFilter {
  public:
    /**
     * @brief Constructor for the MahonyFilter class.
     * @param Kp Proportional gain, i.e. how much the filter uses the
     * accelerometer values to correct for the gyroscope's immediate error.
     * Typically ranges between 0.5 and 5.0.
     * @param Ki Integral gain, i.e. how much the filter corrects for the
     * gyroscope's accumulated error.
     * Typically ranges between 0.0 and 0.1.
     */
    explicit MahonyFilter(float Kp = 1.0f, float Ki = 0.0f) : kp_(Kp), ki_(Ki) {};

    /**
     * @brief Sets the filter's proportional gain.
     * @note Low Kp values can result in slower correction of gyroscope drift.
     *
     * High Kp values can result in overcorrecting the gyroscope values during sharp
     * turns.
     */
    void setKp(float Kp);

    /**
     * @brief Sets the filter's integral gain.
     * @note Low Ki values can result in good response to sharp turns, but suffers more
     * from long-term gyroscope drift.
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
    [[nodiscard]] float getRoll(void) const noexcept;

    /**
     * @brief Returns the current pitch angle, i.e. the rotation around the lateral axis.
     * @return Pitch angle in degrees.
     */
    [[nodiscard]] float getPitch(void) const noexcept;

    /**
     * @brief Returns the current yaw angle, i.e. the rotation around the vertical axis.
     * @return Yaw angle in degrees.
     */
    [[nodiscard]] float getYaw(void) const noexcept;

    /**
     * @brief Resets the filter's internal state to forget any gyroscope bias. Can be used
     * to set a new starting orientation where the roll, pitch, and yaw are all back at 0.
     * @note The proportional gain (Kp) and integral gain (Ki) do not get reset.
     * @warning Not to be used mid-flight unless it's an emergency.
     */
    void reset(void);

  private:
    static constexpr float RAD_TO_DEG = 180.0f / 3.14159265f;

    // Quaternion values
    float q0_ {1.0f};
    float q1_ {};
    float q2_ {};
    float q3_ {};

    // Integral feedback values (accumulates gyroscope error)
    float integralFBx_ {};
    float integralFBy_ {};
    float integralFBz_ {};

    // Proportional gain and integral gain
    float kp_ {1.0f};
    float ki_ {};

    // Euler angles
    float roll_ {};
    float pitch_ {};
    float yaw_ {};

    void computeEulerAngles(void);
};

#endif

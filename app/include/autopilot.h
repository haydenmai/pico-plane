/**
 * @file autopilot.h
 * @brief Automatically maintains the angle and speed of the plane.
 * @author Benley Hsiang
 * @date Sep-24-2026
 */

#ifndef AUTOPILOT_H_
#define AUTOPILOT_H_

#include <cstdint>

namespace Autopilot {

    // Preset target angles
    constexpr float TARGET_ROLL {0.0f};
    constexpr float TARGET_PITCH {10.0f};
    constexpr float TARGET_YAW {0.0f}; // Not actually used at the moment
    constexpr int TARGET_THROTTLE {80};

    void init();
    void cleanup();

    /**
     * @brief Engages the autopilot.
     *
     * @param roll_target Target roll angle (degrees) for the autopilot to reach.
     * @param pitch_target Target pitch angle (degrees) for the autopilot to reach.
     * @param yaw_target Target yaw angle (degrees) for the autopilot to reach.
     * @param throttle_target Target throttle value (1-100) for the autopilot to reach.
     */
    void engage(float roll_target = TARGET_ROLL, float pitch_target = TARGET_PITCH,
                float yaw_target = TARGET_YAW, int throttle_target = TARGET_THROTTLE);

    /**
     * @brief Disengages the autopilot.
     */
    void disengage();

    /**
     * @brief Updates the autopilot control loop.
     *
     * @param roll_current Current roll angle (degrees) from the Mahony filter.
     * @param pitch_current Current pitch angle (degrees) from the Mahony filter.
     * @param yaw_current Current yaw angle (degrees) from the Mahony filter.
     * @param dt_ms Time since the previous autopilot update.
     */
    void update(float roll_current, float pitch_current, float yaw_current,
                uint32_t dt_ms);

    /**
     * @brief Indicates whether autopilot is on or off.
     * @return Returns true if the autopilot is engaged.
     *         Returns false if the autopilot is disengaged.
     */
    [[nodiscard]] bool isEngaged();

    /**
     * @brief Retrieves the autopilot's command for the aileron angle.
     * @return Aileron angle in degrees.
     */
    [[nodiscard]] int getAileronCommand();

    /**
     * @brief Retrieves the autopilot's command for the elevator angle.
     * @return Elevator angle in degrees.
     */
    [[nodiscard]] int getElevatorCommand();

    /**
     * @brief Retrieves the autopilot's command for the rudder angle.
     * @return Rudder angle in degrees.
     */
    [[nodiscard]] int getRudderCommand();

    /**
     * @brief Retrieves the autopilot's command for the throttle percentage.
     * @return Throttle value between 1 and 100.
     */
    [[nodiscard]] int getThrottleCommand();

} // namespace Autopilot

#endif
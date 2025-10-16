/**
 * @file angle_control.h
 * @brief Controls the flight direction of the plane.
 * @author Benley Hsiang
 * @date Oct-04-2025
 */

#ifndef ANGLE_CONTROL_H_
#define ANGLE_CONTROL_H_

#include "hal/servo_ds_m005.h"

namespace AngleController {

    /**
     * @brief Contains the range of angles that a servo is allowed to spin within.
     * @param lower The lower limit of the turning range, in degrees.
     * @param upper The upper limit of the turning range, in degrees.
     */
    struct TurningRange {
        int lower;
        int upper;
    };

    enum ControlType { AILERON, RUDDER, ELEVATOR };

    enum PlaneWing { LEFT, RIGHT };

    /**
     * @brief Initialization function for the AngleController. The setRange function must
     * be called to configure each ControlType servos.
     */
    void init();
    void cleanup();

    /**
     * @brief Sets the range of angles that the given servo can spin within.
     * @param servoType The desired servo(s) to set limits for.
     * @param min The lower limit of the servo's turning range as an angle in degrees.
     * @param max The upper limit of the servo's turning range as an angle in degrees.
     * @pre Angles must be within [MIN_DEG, MAX_DEG].
     *      The lower limit must be less than the upper limit.
     */
    void setRange(ControlType servoType, int min, int max) noexcept;

    /**
     * @brief Retrieves the most recently set turning range of the given servo.
     * @param servoType The desired servo(s) to set limits for.
     * @return TurningRange struct containing the degree limits of the servo.
     */
    [[nodiscard]] TurningRange getRange(ControlType servoType) noexcept;

    /**
     * @brief Commands the given servo(s) to move to a specified angle.
     * @param servoType The desired servo(s) to set the angle for.
     * @param degrees Target angle in degrees.
     * @pre Degrees must be within [lower, upper] for the corresponding struct.
     * @note Values outside [lower, upper] are ignored; no movement occurs.
     */
    void setAngle(ControlType servoType, int degrees) noexcept;

    /**
     * @brief Retrieves the last commanded angle of the given servo.
     * @param servoType The desired servo(s) to get the angle of.
     * @param wing Which plane wing's aileron (left/right) to get the angle of.
     *             This parameter is optional and only used if servoType is AILERON.
     * @return Last set angle in degrees.
     */
    [[nodiscard]] int getAngle(ControlType servoType, PlaneWing wing = LEFT) noexcept;

}; // namespace AngleController

#endif

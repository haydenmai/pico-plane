/**
 * @file angle_control.h
 * @brief Controls the flight direction of the plane.
 * @author Benley Hsiang
 * @date Oct-19-2025
 */

#ifndef ANGLE_CONTROL_H_
#define ANGLE_CONTROL_H_

#include "hal/servo_ds_m005.h"

namespace AngleController {

    /**
     * @brief Contains the center & range of angles that a servo is allowed to spin
     * within.
     * @param center The resting angle of servo arms, in degrees.
     * @param rangeLim The limit of the turning range from the center, in degrees.
     */
    struct TurningLimit {
        int center;
        int rangeLim;

        int upperLim() const { return center + rangeLim; }
        int lowerLim() const { return center - rangeLim; }

        bool isValid() const
        {
            return (upperLim() <= ServoDSM005::MAX_DEG
                    && lowerLim() >= ServoDSM005::MIN_DEG);
        }

        bool isValid(int range) const
        {
            return ((center + range) <= ServoDSM005::MAX_DEG
                    && (center - range) >= ServoDSM005::MIN_DEG);
        }

        bool inRange(int degree) const
        {
            return (degree <= upperLim() && degree >= lowerLim());
        }
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
     * @brief Retrieves the most recently set turning range of the given servo.
     * @param servoType The desired servo(s) to set limits for.
     * @return TurningRange struct containing the degree limits of the servo.
     */
    [[nodiscard]] TurningLimit getTurningLimit(ControlType servoType) noexcept;

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

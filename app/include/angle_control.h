/**
 * @file angle_control.h
 * @brief Controls the flight direction of the plane.
 * @author Benley Hsiang
 * @date Jul-16-2025
 */

#ifndef ANGLE_CONTROL_H_
#define ANGLE_CONTROL_H_

#include "hal/servo_ds_m005.h"

/**
 * @class AngleController
 */
class AngleController {
  public:
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
     * @brief Constructor for the AngleController class.
     * @param aileronRange The range of angles (in degrees) that the ailerons can spin.
     * @param rudderRange The range of angles (in degrees) that the rudder can spin.
     * @param elevatorRange The range of angles (in degrees) that the elevator can spin.
     * @pre Angles must be within [MIN_DEG, MAX_DEG].
     *      The lower limit must be less than the upper limit.
     */
    explicit AngleController(TurningRange aileronRange, TurningRange rudderRange,
                             TurningRange elevatorRange);
    ~AngleController();

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
    [[nodiscard]] TurningRange getRange(ControlType servoType) const noexcept;

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
    [[nodiscard]] int getAngle(ControlType servoType,
                               PlaneWing wing = LEFT) const noexcept;

  private:
    // TODO: Pin numbers are placeholders, change all these later
    static constexpr int AILERON_LEFT_PIN  = 1;
    static constexpr int AILERON_RIGHT_PIN = 2;
    static constexpr int RUDDER_PIN        = 3;
    static constexpr int ELEVATOR_PIN      = 4;
    // TODO: Pin numbers are placeholders, change all these later

    ServoDSM005 aileronLeft_ {AILERON_LEFT_PIN};
    ServoDSM005 aileronRight_ {AILERON_RIGHT_PIN};
    ServoDSM005 rudder_ {RUDDER_PIN};
    ServoDSM005 elevator_ {ELEVATOR_PIN};

    TurningRange aileronLims_;
    TurningRange rudderLims_;
    TurningRange elevatorLims_;

    /**
     * @brief Checks that the given angles are legal values.
     * @param lower The lower limit angle to be verified.
     * @param upper The upper limit angle to be verified.
     * @pre Angles must be within [MIN_DEG, MAX_DEG].
     *      The lower limit must be less than the upper limit.
     * @return True if the angles are valid, false if they're out of range.
     */
    bool rangeIsValid(int lower, int upper);

    /**
     * @brief Checks that the given angle is within the limits for the servo.
     * @param servoType The desired servo(s) to check the angle for.
     * @param angle The proposed angle (in degrees) to set the servo to.
     * @pre Angle must be within [lower, upper] for the corresponding servo.
     * @return True if the angle is within the limits, false if not.
     */
    bool angleIsInRange(ControlType servoType, int angle);

    /**
     * @brief Takes the angle for an aileron and inverts it for the other aileron.
     * @param angle The angle (in degrees) to be inverted.
     * @return Integer representing the inverted angle in degrees.
     */
    int invertAngle(int angle);
};

#endif

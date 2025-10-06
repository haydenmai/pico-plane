/**
 * @file angle_control.cpp
 * @brief Controls the flight direction of the plane.
 * @author Benley Hsiang
 * @date Oct-04-2025
 */

#include "angle_control.h"
#include <cassert>

#include <stdio.h> // Remove later if not needed

namespace AngleController {
    // NOTE: Pin numbers still subject to change
    constexpr int AILERON_LEFT_PIN  = 4;
    constexpr int AILERON_RIGHT_PIN = 5;
    constexpr int RUDDER_PIN        = 6;
    constexpr int ELEVATOR_PIN      = 7;
    // NOTE: Pin numbers still subject to change

    static bool isInitialized = false;

    static ServoDSM005 aileronLeft_ {AILERON_LEFT_PIN};
    static ServoDSM005 aileronRight_ {AILERON_RIGHT_PIN};
    static ServoDSM005 rudder_ {RUDDER_PIN};
    static ServoDSM005 elevator_ {ELEVATOR_PIN};

    static TurningRange aileronLims_;
    static TurningRange rudderLims_;
    static TurningRange elevatorLims_;

    // Local headers
    /**
     * @brief Takes the angle for an aileron and inverts it for the aileron on the
     *        opposite plane wing.
     * @note It is assumed that the default position of the servos is at 90 degrees.
     * @param angle The angle (in degrees) to be inverted.
     * @return Integer representing the inverted angle in degrees.
     */
    static int invertAngle(int angle);

    /**
     * @brief Checks that the given angles are legal values.
     * @param lower The lower limit angle to be verified.
     * @param upper The upper limit angle to be verified.
     * @pre Angles must be within [MIN_DEG, MAX_DEG].
     *      The lower limit must be less than the upper limit.
     * @return True if the angles are valid, false if they're out of range.
     */
    static bool rangeIsValid(int lower, int upper);

    /**
     * @brief Checks that the given angle is within the limits for the servo.
     * @param servoType The desired servo(s) to check the angle for.
     * @param angle The proposed angle (in degrees) to set the servo to.
     * @pre Angle must be within the TurningRange limits for the corresponding servo.
     * @return True if the angle is within the limits, false if not.
     */
    static bool angleIsInRange(ControlType servoType, int angle);

    void init()
    {
        assert(!isInitialized);
        isInitialized = true;
    }

    void cleanup()
    {
        assert(isInitialized);
        isInitialized = false;
    }

    void setRange(ControlType servotype, int min, int max) noexcept
    {
        assert(isInitialized);

        if (!rangeIsValid(min, max)) {
            return;
        }

        switch (servotype) {
        case AILERON:
            aileronLims_.lower = min;
            aileronLims_.upper = max;
            break;

        case RUDDER:
            rudderLims_.lower = min;
            rudderLims_.upper = max;
            break;

        case ELEVATOR:
            elevatorLims_.lower = min;
            elevatorLims_.upper = max;
            break;
        };
    }

    [[nodiscard]] TurningRange getRange(ControlType servoType) noexcept
    {
        assert(isInitialized);

        switch (servoType) {
        case AILERON:
            return aileronLims_;

        case RUDDER:
            return rudderLims_;

        case ELEVATOR:
            return elevatorLims_;
        }

        printf("Error: No recognized servo type was found in getRange().\n");

        return {0, 0};
    }

    void setAngle(ControlType servoType, int degrees) noexcept
    {
        assert(isInitialized);

        if (!angleIsInRange(servoType, degrees)) {
            return;
        }

        switch (servoType) {
        case AILERON:
            aileronLeft_.setAngle(degrees);
            aileronRight_.setAngle(invertAngle(degrees));
            break;

        case RUDDER:
            rudder_.setAngle(degrees);
            break;

        case ELEVATOR:
            elevator_.setAngle(degrees);
            break;
        }
    }

    [[nodiscard]] int getAngle(ControlType servoType, PlaneWing wing) noexcept
    {
        assert(isInitialized);

        switch (servoType) {
        case AILERON:
            return (wing == LEFT) ? aileronLeft_.getAngle() : aileronRight_.getAngle();

        case RUDDER:
            return rudder_.getAngle();

        case ELEVATOR:
            return elevator_.getAngle();
        }

        printf("Error: No recognized servo type was found in getAngle().\n");

        return -1;
    }

    static bool rangeIsValid(int lower, int upper)
    {
        if (lower > upper) {
            printf("Error: Servo's lower limit is not less than the upper limit.\n");
            // TODO: Replace this with exception handling eventually.

            return false;
        }

        if (lower < ServoDSM005::MIN_DEG) {
            printf("Error: Illegal value for the servo's lower limit.\n");
            // TODO: Replace this with exception handling eventually.

            return false;
        }

        if (upper > ServoDSM005::MAX_DEG) {
            printf("Error: Illegal value for the servo's upper limit.\n");
            // TODO: Replace this with exception handling eventually.

            return false;
        }

        return true;
    }

    static bool angleIsInRange(ControlType servoType, int angle)
    {
        int lower {}, upper {};

        switch (servoType) {
        case AILERON:
            lower = aileronLims_.lower;
            upper = aileronLims_.upper;
            break;

        case RUDDER:
            lower = rudderLims_.lower;
            upper = rudderLims_.upper;
            break;

        case ELEVATOR:
            lower = elevatorLims_.lower;
            upper = elevatorLims_.upper;
            break;
        }

        if (angle >= lower && angle <= upper) {
            return true;
        }

        printf("Error: Angle is not in within the set limits.\n");
        // TODO: Replace this with exception handling eventually.

        return false;
    }

    static int invertAngle(int angle) { return ServoDSM005::MAX_DEG - angle; }

} // namespace AngleController
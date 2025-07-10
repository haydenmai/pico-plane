/**
 * @file angle_control.cpp
 * @brief Controls the flight direction of the plane.
 * @author Benley Hsiang
 * @date Jul-09-2025
 */

#include "angle_control.h"

#include "hal/servo_ds_m005.h"

#include <stdio.h> // Remove later if not needed

explicit AngleController::AngleController(TurningRange aileronRange,
                                          TurningRange rudderRange,
                                          TurningRange elevatorRange)
{
    setRange(AILERON, aileronRange.lower, aileronRange.upper);
    setRange(RUDDER, rudderRange.lower, rudderRange.upper);
    setRange(ELEVATOR, elevatorRange.lower, elevatorRange.upper);
}

AngleController::~AngleController() {}

void AngleController::setRange(ControlType servotype, int min, int max) noexcept
{
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

[[nodiscard]] AngleController::TurningRange
AngleController::getRange(ControlType servoType) const noexcept
{
    switch (servoType) {
    case AILERON:
        return aileronLims_;

    case RUDDER:
        return rudderLims_;

    case ELEVATOR:
        return elevatorLims_;
    }
}

void AngleController::setAngle(ControlType servoType, int degrees) noexcept
{
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

[[nodiscard]] int AngleController::getAngle(ControlType servoType,
                                            Direction direction = LEFT) const noexcept
{
    switch (servoType) {
    case AILERON:
        return (direction == LEFT) ? aileronLeft_.getAngle() : aileronRight_.getAngle();

    case RUDDER:
        return rudder_.getAngle();

    case ELEVATOR:
        return elevator_.getAngle();
    }
}

bool AngleController::rangeIsValid(int lower, int upper)
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

bool AngleController::angleIsInRange(ControlType servoType, int angle)
{
    int lower, upper;

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

int AngleController::invertAngle(int angle)
{
    // TODO
}
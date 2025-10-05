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

static bool isInitialized = false;

void init(TurningRange aileronRange, TurningRange rudderRange, TurningRange elevatorRange)
{
    assert(!isInitialized);
    setRange(AILERON, aileronRange.lower, aileronRange.upper);
    setRange(RUDDER, rudderRange.lower, rudderRange.upper);
    setRange(ELEVATOR, elevatorRange.lower, elevatorRange.upper);
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

bool rangeIsValid(int lower, int upper)
{
    assert(isInitialized);

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

bool angleIsInRange(ControlType servoType, int angle)
{
    assert(isInitialized);

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

int invertAngle(int angle)
{
    assert(isInitialized);
    return ServoDSM005::MAX_DEG - angle;
}

// NOTE: Pin numbers still subject to change
static constexpr int AILERON_LEFT_PIN  = 4;
static constexpr int AILERON_RIGHT_PIN = 5;
static constexpr int RUDDER_PIN        = 6;
static constexpr int ELEVATOR_PIN      = 7;
// NOTE: Pin numbers still subject to change

ServoDSM005 aileronLeft_ {AILERON_LEFT_PIN};
ServoDSM005 aileronRight_ {AILERON_RIGHT_PIN};
ServoDSM005 rudder_ {RUDDER_PIN};
ServoDSM005 elevator_ {ELEVATOR_PIN};

TurningRange aileronLims_;
TurningRange rudderLims_;
TurningRange elevatorLims_;

} // namespace AngleController
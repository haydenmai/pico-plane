/**
 * @file angle_control.cpp
 * @brief Controls the flight direction of the plane.
 * @author Benley Hsiang
 * @date Oct-20-2025
 */

#include "angle_control.h"
#include "flight_config.h"

#include <cassert>
#include <stdio.h>

namespace AngleController {
    constexpr TurningLimit AILERON_LIM {FlightConfig::AILERON_CTR_DEG,
                                        FlightConfig::AILERON_RNG_DEG};
    constexpr TurningLimit RUDDER_LIM {FlightConfig::RUDDER_CTR_DEG,
                                       FlightConfig::RUDDER_RNG_DEG};
    constexpr TurningLimit ELEVATOR_LIM {FlightConfig::ELEVATOR_CTR_DEG,
                                         FlightConfig::ELEVATOR_RNG_DEG};

    bool isInitialized = false;

    ServoDSM005 aileronLeft_ {FlightConfig::AILERON_LEFT_PIN};
    ServoDSM005 aileronRight_ {FlightConfig::AILERON_RIGHT_PIN};
    ServoDSM005 rudder_ {FlightConfig::RUDDER_PIN};
    ServoDSM005 elevator_ {FlightConfig::ELEVATOR_PIN};

    // Local headers
    /**
     * @brief Takes the angle for an aileron and inverts it for the aileron on the
     *        opposite plane wing.
     * @note It is assumed that the default position of the servos is at 90 degrees.
     * @param angle The angle (in degrees) to be inverted.
     * @return Integer representing the inverted angle in degrees.
     */
    static int invertAngle(int angle);

    void init()
    {
        assert(!isInitialized);

        // Check if limit is valid, otherwise don't initialize
        if (AILERON_LIM.isValid() && RUDDER_LIM.isValid() && ELEVATOR_LIM.isValid()) {
            isInitialized = true;
        }
    }

    void cleanup()
    {
        assert(isInitialized);
        isInitialized = false;
    }


    [[nodiscard]] TurningLimit getTurningLimit(ControlType servoType) noexcept
    {
        assert(isInitialized);

        switch (servoType) {
        case AILERON:
            return AILERON_LIM;

        case RUDDER:
            return RUDDER_LIM;

        case ELEVATOR:
            return ELEVATOR_LIM;
        }

        printf("Error: No recognized servo type was found in getRange().\n");
        return {0, 0};
    }

    void setAngle(ControlType servoType, int degrees) noexcept
    {
        assert(isInitialized);

        switch (servoType) {
        case AILERON:
            if (!AILERON_LIM.inRange(degrees)) {
                return;
            }
            aileronLeft_.setAngle(degrees);
            aileronRight_.setAngle(degrees + FlightConfig::AILERON_RIGHT_DEG_OFFSET);
            break;

        case RUDDER:
            if (!RUDDER_LIM.inRange(degrees)) {
                return;
            }
            rudder_.setAngle(invertAngle(degrees));
            break;

        case ELEVATOR:
            if (!ELEVATOR_LIM.inRange(degrees)) {
                return;
            }
            elevator_.setAngle(invertAngle(degrees));
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

    static int invertAngle(int angle) { return ServoDSM005::MAX_DEG - angle; }

} // namespace AngleController
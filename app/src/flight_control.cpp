/**
 * @file flight_control.cpp
 * @brief Controls the direction and speed of the plane.
 * @author Benley Hsiang
 * @date Oct-04-2025
 */

#include "flight_control.h"
#include "flight_data.h"
#include <cassert>

namespace FlightController {

    static bool isInitialized_ = false;

    /** @brief Maximum throttle percentage of the motors. */
    static int throttleLimit {10};

    static SpeedController speedCTRL_ {throttleLimit};

    /** @brief Lower and upper limits of the angles the control surfaces can turn. */
    static AngleController::TurningRange aileronRange {70, 110};
    static AngleController::TurningRange rudderRange {70, 110};
    static AngleController::TurningRange elevatorRange {70, 110};


    void init()
    {
        assert(!isInitialized_);
        isInitialized_ = true;
    }

    void cleanup()
    {
        assert(isInitialized_);
        isInitialized_ = false;
    }

    void process_data() noexcept
    {
        assert(isInitialized_);

        // Variables to keep track
        int throttle_curVal {};
        int aileron_curVal {};
        int rudder_curVal {};
        int elevator_curVal {};

        // Process data
        while (1) {
            FlightData::acquire_spinLock();

            int throttle {FlightData::get_throttle()};
            int aileron {FlightData::get_aileron()};
            int rudder {FlightData::get_rudder()};
            int elevator {FlightData::get_elevator()};

            FlightData::release_spinLock();

            // Set new value only if needed
            if (throttle != throttle_curVal) {
                changeSpeed(throttle);
                throttle_curVal = throttle;
            }

            if (aileron != aileron_curVal) {
                changeAngle(AngleController::AILERON, aileron);
                aileron_curVal = aileron;
            }

            if (rudder != rudder_curVal) {
                changeAngle(AngleController::RUDDER, rudder);
                rudder_curVal = rudder;
            }

            if (elevator != elevator_curVal) {
                changeAngle(AngleController::ELEVATOR, elevator);
                elevator_curVal = elevator;
            }
        }
    }


    /**
     * @brief Changes the speed of the plane.
     * @param percent Throttle percentage of the the plane's motors.
     */
    static void changeSpeed(int percent) noexcept
    {
        assert(isInitialized_);
        speedCTRL_.setSpeed(percent);
    }

    /**
     * @brief Turns one of the plane's control surfaces.
     * @param controlType Ailerons, rudder, or elevator.
     * @param angle Angle in degrees to turn to. Resting position is 90 degrees.
     */
    static void changeAngle(AngleController::ControlType controlType, int angle) noexcept
    {
        assert(isInitialized_);
        AngleController::setAngle(controlType, angle);
    }

} // namespace FlightController

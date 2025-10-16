/**
 * @file flight_control.cpp
 * @brief Controls the direction and speed of the plane.
 * @author Benley Hsiang
 * @date Oct-14-2025
 */

#include "flight_control.h"
#include "flight_data.h"
#include "speed_control.h"

#include <cassert>

namespace FlightController {
    /** @brief Maximum throttle percentage of the motors. */
    constexpr int THROTTLE_LIMIT {10};

    /** @brief Lower and upper limits of the angles the control surfaces can turn. */
    // TODO: Values are not in range, need to adjust them
    constexpr AngleController::TurningRange AILERON_RANGE {170, 180};
    constexpr AngleController::TurningRange RUDDER_RANGE {170, 180};
    constexpr AngleController::TurningRange ELEVATOR_RANGE {0, 10};
    // TODO: Values are not in range, need to adjust them

    bool isInitialized_ = false;


    // Local functions headers
    /**
     * @brief Changes the speed of the plane.
     * @param percent Throttle percentage of the the plane's motors.
     */
    static void changeSpeed(int percent) noexcept;

    /**
     * @brief Turns one of the plane's control surfaces.
     * @param controlType Ailerons, rudder, or elevator.
     * @param angle Angle in degrees to turn to. Resting position is 90 degrees.
     */
    static void changeAngle(AngleController::ControlType controlType, int angle) noexcept;

    void init()
    {
        assert(!isInitialized_);

        // Configure speed & angle control limits
        SpeedController::setThrottleLim(THROTTLE_LIMIT);
        AngleController::setRange(AngleController::AILERON, AILERON_RANGE.lower,
                                  AILERON_RANGE.upper);
        AngleController::setRange(AngleController::RUDDER, RUDDER_RANGE.lower,
                                  RUDDER_RANGE.upper);
        AngleController::setRange(AngleController::ELEVATOR, ELEVATOR_RANGE.lower,
                                  ELEVATOR_RANGE.upper);
        
        // Set default flaps to flat with the plane
        changeAngle(AngleController::AILERON, (AILERON_RANGE.upper - AILERON_RANGE.lower) / 2);
        changeAngle(AngleController::RUDDER, (RUDDER_RANGE.upper - RUDDER_RANGE.lower) / 2);
        changeAngle(AngleController::ELEVATOR, (ELEVATOR_RANGE.upper - ELEVATOR_RANGE.lower) / 2);

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

            bool failsafeMode {FlightData::get_FailsafeMode()};

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

            // If controller disconnects, turn off engine
            if (failsafeMode == true) {
                changeSpeed(0);
            }
        }
    }


    static void changeSpeed(int percent) noexcept { SpeedController::setSpeed(percent); }

    static void changeAngle(AngleController::ControlType controlType, int angle) noexcept
    {
        AngleController::setAngle(controlType, angle);
    }

} // namespace FlightController

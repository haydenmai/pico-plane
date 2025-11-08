/**
 * @file flight_control.cpp
 * @brief Controls the direction and speed of the plane.
 * @author Benley Hsiang
 * @date Nov-07-2025
 */

#include "flight_config.h"
#include "flight_control.h"
#include "flight_data.h"
#include "speed_control.h"

#include <cassert>

namespace FlightController {
    bool isInitialized_ = false;


    void init()
    {
        assert(!isInitialized_);

        // Configure speed & angle control limits
        SpeedController::setThrottleLim(FlightConfig::THROTTLE_LIMIT);
        SpeedController::setSpeed(100);

        // Set default position of flaps to be flat with the plane
        AngleController::setAngle(AngleController::AILERON,
                                  FlightConfig::AILERON_CTR_DEG);
        AngleController::setAngle(AngleController::RUDDER, FlightConfig::RUDDER_CTR_DEG);
        AngleController::setAngle(AngleController::ELEVATOR,
                                  FlightConfig::ELEVATOR_CTR_DEG);

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
                SpeedController::setSpeed(throttle);
                throttle_curVal = throttle;
            }

            if (aileron != aileron_curVal) {
                AngleController::setAngle(AngleController::AILERON, aileron);
                aileron_curVal = aileron;
            }

            if (rudder != rudder_curVal) {
                AngleController::setAngle(AngleController::RUDDER, rudder);
                rudder_curVal = rudder;
            }

            if (elevator != elevator_curVal) {
                AngleController::setAngle(AngleController::ELEVATOR, elevator);
                elevator_curVal = elevator;
            }

            // If controller disconnects, turn off engine
            if (failsafeMode == true) {
                SpeedController::setSpeed(0);
            }
        }
    }


} // namespace FlightController

/**
 * @file flight_control.cpp
 * @brief Controls the direction and speed of the plane.
 * @author Benley Hsiang
 * @date May-04-2026
 */

#include "flight_control.h"
#include "flight_config.h"
#include "flight_data.h"
#include "speed_control.h"

#include <cassert>
#include <stdio.h>

namespace FlightController {
    bool isInitialized_ = false;

    void init()
    {
        assert(!isInitialized_);

        // Configure speed & angle control limits
        SpeedController::setThrottleLim(FlightConfig::THROTTLE_LIMIT);
        SpeedController::setSpeed(0);

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

        // Process data
        while (1) {
            FlightData::acquire_spinLock();

            double throttle {FlightData::get_throttle()};
            int aileron {FlightData::get_aileron()};
            int rudder {FlightData::get_rudder()};
            int elevator {FlightData::get_elevator()};
            int toggle {FlightData::get_toggle()};

            bool failsafeMode {FlightData::get_FailsafeMode()};

            FlightData::release_spinLock();

            bool controlsEnabled {toggle >= FlightConfig::CRSF_UPPER};

            if (controlsEnabled) {
                SpeedController::setSpeed(throttle);
                AngleController::setAngle(AngleController::AILERON, aileron);
                AngleController::setAngle(AngleController::RUDDER, rudder);
                AngleController::setAngle(AngleController::ELEVATOR, elevator);
            } else {
                SpeedController::setSpeed(0);
                AngleController::setAngle(AngleController::AILERON,
                                          FlightConfig::AILERON_CTR_DEG);
                AngleController::setAngle(AngleController::RUDDER,
                                          FlightConfig::RUDDER_CTR_DEG);
                AngleController::setAngle(AngleController::ELEVATOR,
                                          FlightConfig::ELEVATOR_CTR_DEG);
            }

            // If controller disconnects, turn off engine
            if (failsafeMode == true) {
                printf("FAILSAFE = TRUE\n");
                SpeedController::setSpeed(0);
            }
        }
    }

} // namespace FlightController

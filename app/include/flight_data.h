/**
 * @file flight_data.h
 * @brief Handles the data coming from the IMU (Inertial Measurement Unit).
 * @author Benley Hsiang, Hayden Mai
 * @date Sep-29-2025
 */

#ifndef FLIGHT_DATA_H_
#define FLIGHT_DATA_H_

#include "crsf/crsf.h"
#include "flight_control.h"
#include "hardware/sync.h"

/**
 * @namespace FlightData
 */
namespace FlightData {
    void init();
    void cleanup();

    // Call this in a loop to get new data from remote control
    void process_frames();

    // Note: To access data in this module,
    //  use the below functions for data-race-safe access
    void acquire_spinLock();
    void release_spinLock();

    [[nodiscard]] const int get_throttle();
    [[nodiscard]] const int get_aileron();
    [[nodiscard]] const int get_elevator();
    [[nodiscard]] const int get_rudder();

}; // namespace FlightData

#endif

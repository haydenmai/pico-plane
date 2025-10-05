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

    void process_frames();

    int get_throttle();
    int get_aileron();
    int get_elevator();
    int get_rudder();

    spin_lock_t *get_spinlock();
}; // namespace FlightData

#endif

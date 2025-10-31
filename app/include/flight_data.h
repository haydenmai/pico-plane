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
 * @brief Handles communication and safe access to remote-control channel data.
 *
 * This module manages CRSF receiver data.
 * It provides initialization, CRSF frame processing, and
 * spinlock-protected access to control channel values.
 */
namespace FlightData {

    /**
     * @brief Initializes the FlightData module.
     *
     * Sets up UART communication, spinlocks, and internal buffers.
     * Must be called before using any other FlightData functions.
     */
    void init();
    void cleanup();

    /**
     * @brief Reads and processes new receiver frames.
     *
     * Should be called frequently (e.g., in the main loop) to update
     * control channel values from the receiver.
     */
    void process_frames();

    /**
     * @brief Acquire the internal spinlock for thread/core-safe access.
     *
     * Use this before manually accessing internal shared data
     * if you need to read or modify values atomically.
     */
    void acquire_spinLock();

    /**
     * @brief Release the internal spinlock previously acquired.
     *
     * Must be called after `acquire_spinLock()` to avoid deadlocks.
     */
    void release_spinLock();

    /**
     * @brief Returns the latest throttle channel value.
     * @return Current throttle value (range: 1000–2000).
     */
    [[nodiscard]] int get_throttle();

    /**
     * @brief Returns the latest aileron channel value.
     * @return Current aileron value (range: 1000–2000).
     */
    [[nodiscard]] int get_aileron();

    /**
     * @brief Returns the latest elevator channel value.
     * @return Current elevator value (range: 1000–2000).
     */
    [[nodiscard]] int get_elevator();

    /**
     * @brief Returns the latest rudder channel value.
     * @return Current rudder value (range 1000–2000).
     */
    [[nodiscard]] int get_rudder();

    [[nodiscard]] bool get_toggle();

    /**
     * @brief Returns the failsafe status (whether the controller is connected).
     * @return true if failsafe mode is active, false otherwise.
     */
    [[nodiscard]] bool get_FailsafeMode();

}; // namespace FlightData

#endif

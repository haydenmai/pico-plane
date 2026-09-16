/**
 * @file flight_control.h
 * @brief Controls the direction and speed of the plane.
 * @author Benley Hsiang
 * @date Sep-10-2026
 */

#ifndef FLIGHT_CONTROL_H_
#define FLIGHT_CONTROL_H_

namespace FlightController {
    void init();
    void cleanup();

    /**
     * @brief Process data from the FlightData module, apply controls to hardware, and
     * filter IMU data.
     */
    void process_and_filter() noexcept;

} // namespace FlightController

#endif

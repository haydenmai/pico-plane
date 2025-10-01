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
 * @class FlightData
 */
class FlightData {
  public:
    explicit FlightData();
    ~FlightData();

    void process_frames() noexcept;

    int get_throttle() noexcept;
    int get_aileron() noexcept;
    int get_elevator() noexcept;
    int get_rudder() noexcept;

    spin_lock_t *get_spinlock() noexcept;

  private:
    static int throttle_val_;
    static int aileron_val_;
    static int elevator_val_;
    static int rudder_val_;

    static spin_lock_t *dataLock_;
    static uint dataLock_num_;

    // Static callbacks (CRSF Module)
    static void on_rc_channels(const uint16_t channels[16]) noexcept;
    static void on_link_stats(const link_statistics_t link_stats) noexcept;
    static void on_failsafe(const bool failsafe) noexcept;

    static int map_to_range2(int range1_val, int range1_min, int range1_max,
                             int range2_min, int range2_max) noexcept;
};

#endif

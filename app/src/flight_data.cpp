/**
 * @file flight_data.cpp
 * @brief Handles the data coming from the IMU (Inertial Measurement Unit).
 * @author Benley Hsiang, Hayden Mai
 * @date Sep-29-2025
 */

#include "flight_data.h"

#include <cassert>
#include <stdio.h>

namespace FlightData {
    static bool isInitialized_ {false};

    static int throttle_val_ {};
    static int aileron_val_ {};
    static int elevator_val_ {};
    static int rudder_val_ {};

    static spin_lock_t *dataLock_ {nullptr};
    static uint dataLock_num_ {0};


    void init()
    {
        assert(!isInitialized_);

        // Acquire a spinlock
        dataLock_num_ = spin_lock_claim_unused(true);
        dataLock_     = spin_lock_instance(dataLock_num_);

        // CRSF setup
        crsf_set_link_quality_threshold(70);
        crsf_set_rssi_threshold(105);

        crsf_set_on_rc_channels(on_rc_channels);
        crsf_set_on_link_statistics(on_link_stats);
        crsf_set_on_failsafe(on_failsafe);

        crsf_begin(uart1, 9, 8);

        isInitialized_ = true;
    }

    void cleanup()
    {
        assert(isInitialized_);
        isInitialized_ = false;
    }

    void process_frames()
    {
        assert(isInitialized_);
        crsf_process_frames();
    }

    int get_throttle()
    {
        assert(isInitialized_);
        return throttle_val_;
    }

    int get_aileron()
    {
        assert(isInitialized_);
        return aileron_val_;
    }

    int get_elevator()
    {
        assert(isInitialized_);
        return elevator_val_;
    }

    int get_rudder()
    {
        assert(isInitialized_);
        return rudder_val_;
    }

    spin_lock_t *get_spinlock()
    {
        assert(isInitialized_);
        return dataLock_;
    }

    static void on_rc_channels(const uint16_t channels[16])
    {
        uint32_t saveState = spin_lock_blocking(dataLock_);

        // Critical section
        throttle_val_ = map_to_range2(TICKS_TO_US(channels[2]), 1000, 2000, 0, 50);
        aileron_val_  = map_to_range2(TICKS_TO_US(channels[0]), 1000, 2000, 70, 110);
        elevator_val_ = map_to_range2(TICKS_TO_US(channels[1]), 1000, 2000, 70, 110);
        rudder_val_   = map_to_range2(TICKS_TO_US(channels[3]), 1000, 2000, 70, 110);

        spin_unlock(dataLock_, saveState);
    }

    static void on_link_stats(const link_statistics_t link_stats)
    {
        printf("RSSI: %d\n", link_stats.rssi);
        printf("Link Quality: %d\n", link_stats.link_quality);
        printf("SNR: %d\n", link_stats.snr);
        printf("TX Power: %d\n", link_stats.tx_power);
    }

    static void on_failsafe(const bool failsafe) { printf("Failsafe: %d\n", failsafe); }

    static int map_to_range2(int range1_val, int range1_min, int range1_max,
                             int range2_min, int range2_max)
    {
        return (range2_max - range2_min) * static_cast<double>(range1_val - range1_min)
                 / static_cast<double>(range1_max - range1_min)
             + range2_min;
    }
} // namespace FlightData
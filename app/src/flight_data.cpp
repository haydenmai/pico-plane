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
    // UART
    constexpr uart_inst_t *UART {uart1};
    constexpr int TX_PIN {8};
    constexpr int RX_PIN {9};

    // Channel indexes
    constexpr int AILERON_IND {0};
    constexpr int ELEVATOR_IND {1};
    constexpr int THROTTLE_IND {2};
    constexpr int RUDDER_IND {3};

    // CRSF Limits set by controller
    constexpr int CRSF_LOWER {1000};
    constexpr int CRSF_UPPER {2000};


    static bool isInitialized_ {false};

    // Read data from CRSF comms
    static int throttle_val_ {};
    static int aileron_val_ {};
    static int elevator_val_ {};
    static int rudder_val_ {};

    // Uses a spinlock
    static spin_lock_t *dataLock_ {nullptr};
    static uint dataLock_num_ {};
    static uint32_t saveState_ {};


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

        crsf_begin(UART, TX_PIN, RX_PIN);

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

    void acquire_spinLock()
    {
        assert(isInitialized_);
        saveState_ = spin_lock_blocking(dataLock_);
    }

    void release_spinLock()
    {
        assert(isInitialized_);
        spin_unlock(dataLock_, saveState_);
    }

    [[nodiscard]] int get_throttle()
    {
        assert(isInitialized_);
        return throttle_val_;
    }

    [[nodiscard]] int get_aileron()
    {
        assert(isInitialized_);
        return aileron_val_;
    }

    [[nodiscard]] int get_elevator()
    {
        assert(isInitialized_);
        return elevator_val_;
    }

    [[nodiscard]] int get_rudder()
    {
        assert(isInitialized_);
        return rudder_val_;
    }


    static void on_rc_channels(const uint16_t channels[16])
    {
        saveState_ = spin_lock_blocking(dataLock_);

        // Critical section
        throttle_val_ = map_to_range2(TICKS_TO_US(channels[THROTTLE_IND]), CRSF_LOWER,
                                      CRSF_UPPER, 0, 50);
        aileron_val_  = map_to_range2(TICKS_TO_US(channels[AILERON_IND]), CRSF_LOWER,
                                      CRSF_UPPER, 70, 110);
        elevator_val_ = map_to_range2(TICKS_TO_US(channels[ELEVATOR_IND]), CRSF_LOWER,
                                      CRSF_UPPER, 70, 110);
        rudder_val_   = map_to_range2(TICKS_TO_US(channels[RUDDER_IND]), CRSF_LOWER,
                                      CRSF_UPPER, 70, 110);

        spin_unlock(dataLock_, saveState_);
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
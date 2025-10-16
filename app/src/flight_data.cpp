/**
 * @file flight_data.cpp
 * @brief Handles the data coming from the receiver and manages
 *        channel access via spinlocks.
  @details Implements CRSF frame handling, synchronization using
 *          hardware spinlocks, and channel value scaling for use
 *          by other modules.
 * @author
 *  - Benley Hsiang,
 *  - Hayden Mai
 * @date Sep-29-2025
 */

#include "flight_control.h"
#include "flight_data.h"

#include <cassert>
#include <stdio.h>

namespace FlightData {
    // UART
    inline static uart_inst_t *UART {uart1};
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


    bool isInitialized_ {false};

    // Saved channel values for throttle, aileron, elevator, and rudder
    int throttle_val_ {};
    int aileron_val_ {};
    int elevator_val_ {};
    int rudder_val_ {};
    bool failsafeMode_ {};

    // Spinlock, index, and interrupt state
    spin_lock_t *dataLock_ {nullptr};
    uint dataLock_num_ {};
    uint32_t saveState_ {};


    // Local functions headers
    /**
     * @brief Callback triggered when a new RC channel frame is received.
     * @param channels Array of 16 channel values in CRSF tick format.
     */
    static void on_rc_channels(const uint16_t channels[16]);

    /**
     * @brief Callback triggered when link statistics are updated.
     * @param link_stats Current CRSF link quality and signal metrics.
     */
    static void on_link_stats(const link_statistics_t link_stats);

    /**
     * @brief Callback triggered when failsafe mode is entered or exited.
     * @param failsafe True if failsafe is active.
     */
    static void on_failsafe(const bool failsafe);

    /**
     * @brief Maps a value from one numerical range to another.
     * @param range1_val Input value in range1.
     * @param range1_min Minimum of input range.
     * @param range1_max Maximum of input range.
     * @param range2_min Minimum of output range.
     * @param range2_max Maximum of output range.
     * @return Scaled value in the new range.
     */
    static int map_to_range2(int range1_val, int range1_min, int range1_max,
                             int range2_min, int range2_max);

    // static void set_battery();

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

    [[nodiscard]] bool get_FailsafeMode()
    {
        assert(isInitialized_);
        return failsafeMode_;
    }

    static void on_rc_channels(const uint16_t channels[16])
    {
        saveState_ = spin_lock_blocking(dataLock_);

        // Critical section
        throttle_val_ = map_to_range2(TICKS_TO_US(channels[THROTTLE_IND]), CRSF_LOWER,
                                      CRSF_UPPER, 0, FlightController::THROTTLE_LIMIT);
        aileron_val_  = map_to_range2(TICKS_TO_US(channels[AILERON_IND]), CRSF_LOWER,
                                      CRSF_UPPER, FlightController::AILERON_RANGE.lower,
                                      FlightController::AILERON_RANGE.upper);
        elevator_val_ = map_to_range2(TICKS_TO_US(channels[ELEVATOR_IND]), CRSF_LOWER,
                                      CRSF_UPPER, FlightController::ELEVATOR_RANGE.lower,
                                      FlightController::ELEVATOR_RANGE.upper);
        rudder_val_   = map_to_range2(TICKS_TO_US(channels[RUDDER_IND]), CRSF_LOWER,
                                      CRSF_UPPER, FlightController::RUDDER_RANGE.lower,
                                      FlightController::RUDDER_RANGE.upper);

        spin_unlock(dataLock_, saveState_);
    }

    static void on_link_stats(const link_statistics_t link_stats)
    {
        printf("RSSI: %d\n", link_stats.rssi);
        printf("Link Quality: %d\n", link_stats.link_quality);
        printf("SNR: %d\n", link_stats.snr);
        printf("TX Power: %d\n", link_stats.tx_power);
    }

    static void on_failsafe(const bool failsafe) { failsafeMode_ = failsafe; }

    static int map_to_range2(int range1_val, int range1_min, int range1_max,
                             int range2_min, int range2_max)
    {
        return (range2_max - range2_min) * static_cast<double>(range1_val - range1_min)
                 / static_cast<double>(range1_max - range1_min)
             + range2_min;
    }

    // static void set_battery() { crsf_telem_set_battery_data(0, 0, 0, 0); }

} // namespace FlightData
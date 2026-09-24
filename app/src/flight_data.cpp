/**
 * @file flight_data.cpp
 * @brief Handles the data coming from the receiver and manages
 *        channel access via spinlocks.
 * @details Implements CRSF frame handling, synchronization using
 *          hardware spinlocks, and channel value scaling for use
 *          by other modules.
 * @author Benley Hsiang, Hayden Mai
 * @date Sep-23-2026
 */

#include "flight_data.h"
#include "flight_config.h"
#include "flight_control.h"

#include "pico/time.h"

#include <cassert>
#include <stdio.h>

namespace FlightData {
    bool isInitialized_ {false};

    // Saved channel values for throttle, aileron, elevator, and rudder
    double throttle_val_ {};
    int aileron_val_ {};
    int elevator_val_ {};
    int rudder_val_ {};
    int toggle_val_ {FlightConfig::CRSF_LOWER};
    int autopilot_val_ {FlightConfig::CRSF_LOWER};
    bool failsafeMode_ {false};
    static constexpr int64_t FAILSAFE_HOLD_US {1 * 1000 * 1000};

    bool badLinkTiming_ {false};
    absolute_time_t badLinkSince_ {};

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
     * @brief Callback triggered when CRSF enters or exits failsafe.
     * @param failsafe True if CRSF currently reports a failsafe condition.
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

    static double map_to_range2f(int range1_val, int range1_min, int range1_max,
                                 int range2_min, int range2_max);

    // static void set_battery();

    void init()
    {
        assert(!isInitialized_);

        // Acquire a spinlock
        dataLock_num_ = spin_lock_claim_unused(true);
        dataLock_     = spin_lock_instance(dataLock_num_);

        crsf_set_on_rc_channels(on_rc_channels);
        crsf_set_on_link_statistics(on_link_stats);
        crsf_set_on_failsafe(on_failsafe);

        // CRSF starts in failsafe; the callback only fires on change.
        badLinkSince_  = get_absolute_time();
        badLinkTiming_ = true;

        crsf_begin(FlightConfig::UART, FlightConfig::UART_RX_PIN,
                   FlightConfig::UART_TX_PIN);

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

        if (badLinkTiming_ && !failsafeMode_
            && absolute_time_diff_us(badLinkSince_, get_absolute_time())
                   >= FAILSAFE_HOLD_US) {
            failsafeMode_ = true;
        }
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

    [[nodiscard]] double get_throttle()
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

    [[nodiscard]] int get_toggle()
    {
        assert(isInitialized_);
        return toggle_val_;
    }

    [[nodiscard]] int get_autopilot()
    {
        assert(isInitialized_);
        return autopilot_val_;
    }

    static void on_rc_channels(const uint16_t channels[16])
    {
        saveState_ = spin_lock_blocking(dataLock_);

        // Critical section
        throttle_val_ = map_to_range2f(TICKS_TO_US(channels[FlightConfig::THROTTLE_IND]),
                                       FlightConfig::CRSF_LOWER, FlightConfig::CRSF_UPPER,
                                       0, FlightConfig::THROTTLE_LIMIT);

        aileron_val_
            = map_to_range2(TICKS_TO_US(channels[FlightConfig::AILERON_IND]),
                            FlightConfig::CRSF_LOWER, FlightConfig::CRSF_UPPER,
                            getTurningLimit(AngleController::AILERON).lowerLim(),
                            getTurningLimit(AngleController::AILERON).upperLim());

        elevator_val_
            = map_to_range2(TICKS_TO_US(channels[FlightConfig::ELEVATOR_IND]),
                            FlightConfig::CRSF_LOWER, FlightConfig::CRSF_UPPER,
                            getTurningLimit(AngleController::ELEVATOR).lowerLim(),
                            getTurningLimit(AngleController::ELEVATOR).upperLim());

        rudder_val_ = map_to_range2(TICKS_TO_US(channels[FlightConfig::RUDDER_IND]),
                                    FlightConfig::CRSF_LOWER, FlightConfig::CRSF_UPPER,
                                    getTurningLimit(AngleController::RUDDER).lowerLim(),
                                    getTurningLimit(AngleController::RUDDER).upperLim());

        toggle_val_    = TICKS_TO_US(channels[FlightConfig::TOGGLE_IND]);
        autopilot_val_ = TICKS_TO_US(channels[FlightConfig::AUTOPILOT_IND]);

        spin_unlock(dataLock_, saveState_);
    }

    static void on_link_stats(const link_statistics_t link_stats)
    {
        // printf("RSSI: %d\n", link_stats.rssi);
        // printf("Link Quality: %d\n", link_stats.link_quality);
        // printf("SNR: %d\n", link_stats.snr);
        // printf("TX Power: %d\n", link_stats.tx_power);
    }

    static void on_failsafe(const bool failsafe)
    {
        if (!failsafe) {
            badLinkTiming_ = false;
            failsafeMode_  = false;
            return;
        }

        if (!badLinkTiming_) {
            badLinkSince_  = get_absolute_time();
            badLinkTiming_ = true;
        }
    }

    static int map_to_range2(int range1_val, int range1_min, int range1_max,
                             int range2_min, int range2_max)
    {
        return range2_min
             + (static_cast<double>(range2_max - range2_min)
                * static_cast<double>(range1_val - range1_min))
                   / static_cast<double>(range1_max - range1_min);
    }

    static double map_to_range2f(int range1_val, int range1_min, int range1_max,
                                 int range2_min, int range2_max)
    {
        return static_cast<double>(range2_min)
             + (static_cast<double>(range2_max - range2_min)
                * static_cast<double>(range1_val - range1_min))
                   / static_cast<double>(range1_max - range1_min);
    }

    // static void set_battery() { crsf_telem_set_battery_data(0, 0, 0, 0); }

} // namespace FlightData
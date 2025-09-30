/**
 * @file flight_data.cpp
 * @brief Handles the data coming from the IMU (Inertial Measurement Unit).
 * @author Benley Hsiang, Hayden Mai
 * @date Sep-29-2025
 */

#include "flight_data.h"
#include "crsf/crsf.h"

#include <stdio.h>

FlightData::FlightData()
{
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
}

FlightData::~FlightData() {}

void FlightData::process_frames() noexcept { crsf_process_frames(); }

int FlightData::get_throttle() noexcept { return throttle_val_; }

int FlightData::get_aileron() noexcept { return aileron_val_; }

int FlightData::get_elevator() noexcept { return elevator_val_; }

int FlightData::get_rudder() noexcept { return elevator_val_; }

spin_lock_t *FlightData::get_spinlock() noexcept { return dataLock_; }

void FlightData::on_rc_channels(const uint16_t channels[16]) noexcept
{
    uint32_t saveState = spin_lock_blocking(dataLock_);

    // Critical section
    throttle_val_ = map_to_range2(TICKS_TO_US(channels[2]), 1000, 2000, 0, 50);

    aileron_val_  = map_to_range2(TICKS_TO_US(channels[0]), 1000, 2000, 70, 110);
    elevator_val_ = map_to_range2(TICKS_TO_US(channels[1]), 1000, 2000, 70, 110);
    rudder_val_   = map_to_range2(TICKS_TO_US(channels[3]), 1000, 2000, 70, 110);

    spin_unlock(dataLock_, saveState);
}

void FlightData::on_link_stats(const link_statistics_t link_stats) noexcept
{
    printf("RSSI: %d\n", link_stats.rssi);
    printf("Link Quality: %d\n", link_stats.link_quality);
    printf("SNR: %d\n", link_stats.snr);
    printf("TX Power: %d\n", link_stats.tx_power);
}

void on_failsafe(const bool failsafe) noexcept { printf("Failsafe: %d\n", failsafe); }

int FlightData::map_to_range2(int range1_val, int range1_min, int range1_max,
                              int range2_min, int range2_max) noexcept
{
    return (range2_max - range2_min) * static_cast<double>(range1_val - range1_min)
             / static_cast<double>(range1_max - range1_min)
         + range2_min;
}
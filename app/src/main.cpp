/**
 * @file main.cpp
 *
 * @author Hayden Mai, Benley Hsiang
 * @brief Controls an airplane and data
 * @date Aug-22-2025
 */

// hal layer 
#include "crsf/crsf.h"
#include "hardware/pwm.h"
#include "pico/stdlib.h"

// app layer
#include "flight_control.h"

// SDK
#include "hal/motor_esc.h"
#include "hal/mpu6050.h"
#include "hal/pico_led.h"
#include "hal/pwm_led.h"
#include "hal/servo_ds_m005.h"
#include <stdio.h>


static auto flightController = FlightController();

int map_to_range2(int range1_val, int range1_min, int range1_max, int range2_min,
                  int range2_max)
{
    return (range2_max - range2_min) * static_cast<double>(range1_val - range1_min)
             / static_cast<double>(range1_max - range1_min)
         + range2_min;
}

void on_rc_channels(const uint16_t channels[16])
{
	// TODO: Do the controls on the other core
    int throttle {map_to_range2(TICKS_TO_US(channels[2]), 1000, 2000, 0, 50)};

    int aileron {map_to_range2(TICKS_TO_US(channels[0]), 1000, 2000, 70, 110)};
    int elevator {map_to_range2(TICKS_TO_US(channels[1]), 1000, 2000, 70, 110)};
    int rudder {map_to_range2(TICKS_TO_US(channels[3]), 1000, 2000, 70, 110)};

    flightController.changeSpeed(throttle);

    flightController.changeAngle(AngleController::AILERON, aileron);
    flightController.changeAngle(AngleController::RUDDER, rudder);
    flightController.changeAngle(AngleController::ELEVATOR, elevator);
}

void on_link_stats(const link_statistics_t link_stats)
{
    printf("RSSI: %d\n", link_stats.rssi);
    printf("Link Quality: %d\n", link_stats.link_quality);
    printf("SNR: %d\n", link_stats.snr);
    printf("TX Power: %d\n", link_stats.tx_power);
}

void on_failsafe(const bool failsafe) { printf("Failsafe: %d\n", failsafe); }

int main()
{
    stdio_init_all();

    crsf_set_link_quality_threshold(70);
    crsf_set_rssi_threshold(105);

    crsf_set_on_rc_channels(on_rc_channels);
    crsf_set_on_link_statistics(on_link_stats);
    crsf_set_on_failsafe(on_failsafe);

    crsf_begin(uart1, 9, 8);

    for (;;) {
        crsf_process_frames();
	}

    stdio_deinit_all();

    return 0;
}

void set_battery() { crsf_telem_set_battery_data(0, 0, 0, 0); }
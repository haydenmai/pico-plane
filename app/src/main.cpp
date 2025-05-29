/**
 * @file main.cpp
 *
 * @author Hayden Mai
 * @brief Controls an airplane and data
 * @date May-07-2025
 */

#include "crsf/crsf.h"
#include "hal/pwm_led.h"
#include "hardware/pwm.h"
#include "pico/stdlib.h"

#include <stdio.h>

auto led16 = pwmLED(16);
auto led17 = pwmLED(17);
auto led18 = pwmLED(18);
auto led19 = pwmLED(19);
auto led20 = pwmLED(20);

void on_rc_channels(const uint16_t channels[16])
{
    printf("Channel 1: %f\n", TICKS_TO_US(channels[0]));
    printf("Channel 2: %f\n", TICKS_TO_US(channels[1]));
    printf("Channel 3: %f\n", TICKS_TO_US(channels[2]));
    printf("Channel 4: %f\n", TICKS_TO_US(channels[3]));
    printf("Channel 5: %f\n", TICKS_TO_US(channels[4]));
    printf("Channel 6: %f\n", TICKS_TO_US(channels[5]));
    printf("Channel 7: %f\n", TICKS_TO_US(channels[6]));
    printf("Channel 8: %f\n", TICKS_TO_US(channels[7]));
    printf("Channel 9: %f\n", TICKS_TO_US(channels[8]));
    printf("Channel 10: %f\n", TICKS_TO_US(channels[9]));
    printf("Channel 11: %f\n", TICKS_TO_US(channels[10]));
    printf("Channel 12: %f\n", TICKS_TO_US(channels[11]));
    //printf("Channel 13: %f\n", TICKS_TO_US(channels[12]));
    //printf("Channel 14: %f\n", TICKS_TO_US(channels[13]));
    //printf("Channel 15: %f\n", TICKS_TO_US(channels[14]));
    //printf("Channel 16: %f\n", TICKS_TO_US(channels[15]));

    led16.setBrightness(TICKS_TO_US(channels[0]));
    led17.setBrightness(TICKS_TO_US(channels[1]));
    led18.setBrightness(TICKS_TO_US(channels[2]));
    led19.setBrightness(TICKS_TO_US(channels[3]));
    led20.setBrightness(TICKS_TO_US(channels[11]));
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
    for (;;)
        crsf_process_frames();
}

void set_battery() { crsf_telem_set_battery_data(0, 0, 0, 0); }
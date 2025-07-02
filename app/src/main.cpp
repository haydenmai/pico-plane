/**
 * @file main.cpp
 *
 * @author Hayden Mai, Benley Hsiang
 * @brief Controls an airplane and data
 * @date Jul-02-2025
 */

#include "crsf/crsf.h"
#include "hardware/pwm.h"
#include "pico/stdlib.h"

#include "hal/motor_esc.h"
#include "hal/mpu6050.h"
#include "hal/pico_led.h"
#include "hal/pwm_led.h"
#include "hal/servo_ds_m005.h"
#include <stdio.h>

auto esc18 = MotorEsc(18, 100); // Throttle limit 5% for now

// auto led17 = pwmLED(17);
// auto led18 = pwmLED(18);
// auto led19 = pwmLED(19);

int map_to_range2(int range1_val, int range1_min, int range1_max, int range2_min,
                  int range2_max)
{
    return (range2_max - range2_min) * static_cast<double>(range1_val - range1_min)
             / static_cast<double>(range1_max - range1_min)
         + range2_min;
}


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
    printf("\n");
    // printf("Channel 13: %f\n", TICKS_TO_US(channels[12]));
    // printf("Channel 14: %f\n", TICKS_TO_US(channels[13]));
    // printf("Channel 15: %f\n", TICKS_TO_US(channels[14]));
    // printf("Channel 16: %f\n", TICKS_TO_US(channels[15]));

    // int deg1 {map_to_range2(TICKS_TO_US(channels[0]), 1000, 2000, 0, 180)};
    // int deg2 {map_to_range2(TICKS_TO_US(channels[1]), 1000, 2000, 0, 180)};
    // int deg3 {map_to_range2(TICKS_TO_US(channels[3]), 1000, 2000, 0, 180)};

    int throttle {map_to_range2(TICKS_TO_US(channels[2]), 1000, 2000, 0, 100)};

    esc18.setSpeed(throttle);

    // srv14.setAngle(deg1);
    // srv15.setAngle(deg2);
    // srv16.setAngle(deg3);

    // led17.setBrightness(TICKS_TO_US(channels[2]));
    // led18.setBrightness(TICKS_TO_US(channels[4]));
    // led19.setBrightness(TICKS_TO_US(channels[5]));
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
    /**
        sleep_ms(10000);

        servo.setAngle(0);

        MPU6050 joe = MPU6050();

        while (true) {

            MPU6050::AccelVal joemama = joe.getAccelValues();
            MPU6050::GyroVal amogus   = joe.getGyroValues();

            printf("\nx: %.3fg, y: %.3fg, z: %.3fg\n", joemama.x, joemama.y, joemama.z);
            printf("\nx: %.3f deg/sec, y: %.3f deg/sec, z: %.3f deg/sec,\n", amogus.x,
                   amogus.y, amogus.z);

            onboard_led.on();
            sleep_ms(50);
            onboard_led.off();
            sleep_ms(50);
        }
        **/

    stdio_deinit_all();

    return 0;
}

void set_battery() { crsf_telem_set_battery_data(0, 0, 0, 0); }
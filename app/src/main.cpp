/**
 * @file main.cpp
 *
 * @author Hayden Mai, Benley Hsiang
 * @brief Controls an airplane and data
 * @date Aug-08-2026
 */

// SDK
#include "hardware/clocks.h"
#include "hardware/pwm.h"
#include "pico/multicore.h"
#include "pico/stdlib.h"

// Set to 1 to run ESC calibration sequence at startup (max->min throttle).
#ifndef ESC_CALIBRATE_ON_START
#define ESC_CALIBRATE_ON_START 0
#endif

// app layer
#include "angle_control.h"
#include "flight_control.h"
#include "flight_data.h"
#include "mahony_filter.h"
#include "speed_control.h"

// hal layer
#include "crsf/crsf.h"
#include "hal/motor_esc.h"
#include "hal/mpu6050.h"
#include "hal/pico_led.h"
#include "hal/pwm_led.h"
#include "hal/servo_ds_m005.h"

#include <cstdio> // For testing with printf()

int main()
{
    /*
        stdio_init_all();
        SpeedController::init();
    #if ESC_CALIBRATE_ON_START
        // Calibration mode is standalone: do not start normal flight control.
        SpeedController::calibrate();
        while (1) {
            tight_loop_contents();
        }
    #else
        AngleController::init();
        FlightController::init();
        FlightData::init();

        multicore_launch_core1(FlightController::process_data);

        // Handle receiving data
        while (1) {
            FlightData::process_frames();
        }
    #endif

        FlightData::cleanup();
        FlightController::cleanup();
        SpeedController::cleanup();
        AngleController::cleanup();

        stdio_deinit_all();
    */

    stdio_init_all();
    sleep_ms(10000);

    auto &onboard_led = PicoLED::get();
    bool configured   = set_sys_clock_khz(200000, true);
    if (!configured) {
        while (true) {
            onboard_led.on();
            sleep_ms(50);
            onboard_led.off();
            sleep_ms(50);
        }
    }

    MPU6050 sensor      = MPU6050();
    MahonyFilter filter = MahonyFilter();

    absolute_time_t prev_time = get_absolute_time();
    const float DEG_TO_RAD    = 3.14159265f / 180.0f;

    int counter = 0;

    while (true) {
        absolute_time_t now = get_absolute_time();
        float dt            = (float)absolute_time_diff_us(prev_time, now) / 1000000.0f;
        prev_time           = now;
        if (dt < 0.0001f) {
            dt = 0.0001f;
        }

        MPU6050::AccelVal raw_accel = sensor.getAccelValues();
        MPU6050::GyroVal raw_gyro   = sensor.getGyroValues();

        float gx = raw_gyro.x * DEG_TO_RAD;
        float gy = raw_gyro.y * DEG_TO_RAD;
        float gz = raw_gyro.z * DEG_TO_RAD;

        filter.update(raw_accel.x, raw_accel.y, raw_accel.z, gx, gy, gz, dt);

        if (counter % 2 == 0) {
            printf("%.2f, %.2f, %.2f\n", filter.getRoll(), filter.getPitch(),
                   filter.getYaw());
            counter = 0;
        }
        counter++;
        sleep_ms(5);
    }

    stdio_deinit_all();
    return 0;
}

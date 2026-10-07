/**
 * @file main.cpp
 *
 * @author Hayden Mai, Benley Hsiang
 * @brief Controls an airplane and data
 * @date Sep-23-2026
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
#include "autopilot.h"
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

int main()
{
    stdio_init_all();

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

    SpeedController::init();
#if ESC_CALIBRATE_ON_START
    // Calibration mode is standalone: do not start normal flight control.
    sleep_ms(1000);
    SpeedController::calibrate();
    while (1) {
        tight_loop_contents();
    }
#else
    AngleController::init();
    FlightController::init();
    Autopilot::init();
    FlightData::init();

    multicore_launch_core1(FlightController::process_and_filter);

    // Handle receiving data
    while (1) {
        FlightData::process_frames();
    }
#endif

    FlightData::cleanup();
    Autopilot::cleanup();
    FlightController::cleanup();
    SpeedController::cleanup();
    AngleController::cleanup();

    stdio_deinit_all();
    return 0;
}

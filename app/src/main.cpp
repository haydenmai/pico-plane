/**
 * @file main.cpp
 *
 * @author Hayden Mai, Benley Hsiang
 * @brief Controls an airplane and data
 * @date Apr-30-2026
 */

// SDK
#include "hardware/pwm.h"
#include "pico/multicore.h"
#include "pico/stdlib.h"

// Set to 1 to run ESC calibration sequence at startup (max->min throttle).
#ifndef ESC_CALIBRATE_ON_START
#define ESC_CALIBRATE_ON_START 1
#endif

// app layer
#include "angle_control.h"
#include "flight_control.h"
#include "flight_data.h"
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

    return 0;
}

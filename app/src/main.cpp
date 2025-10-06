/**
 * @file main.cpp
 *
 * @author Hayden Mai, Benley Hsiang
 * @brief Controls an airplane and data
 * @date Aug-22-2025
 */

// SDK
#include "hardware/pwm.h"
#include "pico/multicore.h"
#include "pico/stdlib.h"

// app layer
#include "flight_control.h"
#include "flight_data.h"

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
    FlightData::init();
    FlightController::init();

    multicore_launch_core1(FlightController::process_data);

    // Handle receiving data
    while (1) {
        FlightData::process_frames();
    }

    FlightData::cleanup();
    stdio_deinit_all();

    return 0;
}

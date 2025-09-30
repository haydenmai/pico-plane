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
#include "pico/multicore.h"
#include "pico/stdlib.h"

// app layer
#include "flight_control.h"
#include "flight_data.h"

// SDK
#include "hal/motor_esc.h"
#include "hal/mpu6050.h"
#include "hal/pico_led.h"
#include "hal/pwm_led.h"
#include "hal/servo_ds_m005.h"
#include <stdio.h>

static auto flightData = FlightData();

/**
 * @brief Control hardware via core 1.
 */
void core1_entry(void)
{
    auto flightController = FlightController();

    // Retrieve spinlock from FlightData class
    spin_lock_t *spinLock = flightData.get_spinlock();

    int throttle_curVal {};
    int aileron_curVal {};
    int rudder_curVal {};
    int elevator_curVal {};

	// Process data
    while (1) {
        uint32_t saveState = spin_lock_blocking(spinLock);

        int throttle {flightData.get_throttle()};
        int aileron {flightData.get_aileron()};
        int rudder {flightData.get_rudder()};
        int elevator {flightData.get_elevator()};

        spin_unlock(spinLock, saveState);

        // Set new value only if needed
        if (throttle != throttle_curVal) {
            flightController.changeSpeed(throttle);
            throttle_curVal = throttle;
        }

        if (aileron != aileron_curVal) {
            flightController.changeAngle(AngleController::AILERON, aileron);
            aileron_curVal = aileron;
        }

        if (rudder != rudder_curVal) {
            flightController.changeAngle(AngleController::RUDDER, rudder);
            rudder_curVal = rudder;
        }

        if (elevator != elevator_curVal) {
            flightController.changeAngle(AngleController::ELEVATOR, elevator);
            elevator_curVal = elevator;
        }
    }
}

int main()
{
    stdio_init_all();
    multicore_launch_core1(core1_entry);

	// Handle receiving data
    while (1) {
        flightData.process_frames();
    }

    stdio_deinit_all();

    return 0;
}

void set_battery() { crsf_telem_set_battery_data(0, 0, 0, 0); }
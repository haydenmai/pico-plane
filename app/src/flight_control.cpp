/**
 * @file flight_control.cpp
 * @brief Controls the direction and speed of the plane.
 * @author Benley Hsiang
 * @date Sep-10-2026
 */

// SDK
#include "hardware/clocks.h"
#include "pico/stdlib.h"
#include "pico/time.h"

// app layer
#include "flight_config.h"
#include "flight_control.h"
#include "flight_data.h"
#include "mahony_filter.h"
#include "speed_control.h"

// hal layer
#include "hal/mpu6050.h"
#include "hal/pico_led.h"

#include <cassert>
#include <cstdio>

// #define DEBUG
#ifdef DEBUG
#define DEBUG_PRINT(roll, pitch, yaw, counter)                                           \
    do {                                                                                 \
        if ((counter) % 2 == 0) {                                                        \
            printf("%.2f, %.2f, %.2f\n", roll, pitch, yaw);                              \
            (counter) = 0;                                                               \
        }                                                                                \
        (counter)++;                                                                     \
    } while (0)
#else
#define DEBUG_PRINT(unused1, unused2, unused3, unused4)                                  \
    do {                                                                                 \
    } while (0)
#endif

namespace FlightController {
    bool isInitialized_ {false};

    MPU6050 imu               = MPU6050();
    MahonyFilter filter       = MahonyFilter();
    absolute_time_t prev_time = get_absolute_time();
    const float DEG_TO_RAD    = 3.14159265f / 180.0f;

#ifdef DEBUG
    static int counter = 0;
#endif

    void init()
    {
        assert(!isInitialized_);

        // Configure speed & angle control limits
        SpeedController::setThrottleLim(FlightConfig::THROTTLE_LIMIT);
        SpeedController::setSpeed(0);

        // Set default position of flaps to be flat with the plane
        AngleController::setAngle(AngleController::AILERON,
                                  FlightConfig::AILERON_CTR_DEG);
        AngleController::setAngle(AngleController::RUDDER, FlightConfig::RUDDER_CTR_DEG);
        AngleController::setAngle(AngleController::ELEVATOR,
                                  FlightConfig::ELEVATOR_CTR_DEG);

        isInitialized_ = true;
    }

    void cleanup()
    {
        assert(isInitialized_);
        isInitialized_ = false;
    }

    void process_data() noexcept
    {
        assert(isInitialized_);

        // Process data
        FlightData::acquire_spinLock();

        double throttle {FlightData::get_throttle()};
        int aileron {FlightData::get_aileron()};
        int rudder {FlightData::get_rudder()};
        int elevator {FlightData::get_elevator()};
        int toggle {FlightData::get_toggle()};

        bool failsafeMode {FlightData::get_FailsafeMode()};

        FlightData::release_spinLock();

        bool controlsEnabled {toggle >= FlightConfig::CRSF_UPPER};

        if (controlsEnabled) {
            SpeedController::setSpeed(throttle);
            AngleController::setAngle(AngleController::AILERON, aileron);
            AngleController::setAngle(AngleController::RUDDER, rudder);
            AngleController::setAngle(AngleController::ELEVATOR, elevator);
        } else {
            SpeedController::setSpeed(0);
            AngleController::setAngle(AngleController::AILERON,
                                      FlightConfig::AILERON_CTR_DEG);
            AngleController::setAngle(AngleController::RUDDER,
                                      FlightConfig::RUDDER_CTR_DEG);
            AngleController::setAngle(AngleController::ELEVATOR,
                                      FlightConfig::ELEVATOR_CTR_DEG);
        }

        // If controller disconnects, turn off engine
        if (failsafeMode == true) {
            printf("FAILSAFE = TRUE\n");
            SpeedController::setSpeed(0);
        }
    }

    void filter_imu_data() noexcept
    {
        assert(isInitialized_);

        absolute_time_t now = get_absolute_time();
        float dt            = (float)absolute_time_diff_us(prev_time, now) / 1000000.0f;
        prev_time           = now;
        if (dt < 0.0001f) {
            dt = 0.0001f;
        }

        MPU6050::AccelVal raw_accel = imu.getAccelValues();
        MPU6050::GyroVal raw_gyro   = imu.getGyroValues();

        float gx = raw_gyro.x * DEG_TO_RAD;
        float gy = raw_gyro.y * DEG_TO_RAD;
        float gz = raw_gyro.z * DEG_TO_RAD;

        filter.update(raw_accel.x, raw_accel.y, raw_accel.z, gx, gy, gz, dt);

        DEBUG_PRINT(filter.getRoll(), filter.getPitch(), filter.getYaw(), counter);
    }

    void process_and_filter() noexcept
    {
        assert(isInitialized_);

        while (1) {
            filter_imu_data();
            process_data();
            sleep_ms(5);
        }
    }
} // namespace FlightController

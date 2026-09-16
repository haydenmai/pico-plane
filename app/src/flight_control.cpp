/**
 * @file flight_control.cpp
 * @brief Controls the direction and speed of the plane.
 * @author Benley Hsiang
 * @date Sep-15-2026
 */

// SDK
#include "pico/stdlib.h"
#include "pico/time.h"

// app layer
#include "autopilot.h"
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

// #define DEBUG // Comment out to disable debug mode
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

    const float DEG_TO_RAD = 3.14159265f / 180.0f;
    const float US_PER_SEC {1000000.0f};
    const float DT_LOWER_BOUND {0.0001f};
    const float DT_UPPER_BOUND {0.05f};

    absolute_time_t prev_filter_time {};
    absolute_time_t prev_process_time {};

    // Controls speed of the data processing/filtering function calls, adjust as needed
    const uint32_t FILTER_FREQ_US {5000};
    const uint32_t PROCESS_FREQ_US {10000};

    // Angle info for autopilot
    static float roll {};
    static float pitch {};
    static float yaw {};

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

    void process_data(uint32_t dt_ms) noexcept
    {
        assert(isInitialized_);

        static bool wasFailSafe {false};
        static bool autopilotWasRequested {false};
        bool autopilotBeingRequested {false}; // TODO

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

        if (autopilotBeingRequested && !autopilotWasRequested) {
            Autopilot::engage(roll, pitch, yaw, throttle);
        } else if (!autopilotBeingRequested && autopilotWasRequested) {
            Autopilot::disengage();
        }
        autopilotWasRequested = autopilotBeingRequested;

        // If controller disconnects, turn off engine
        // Autopilot will have to be turned off and on to be used again.
        if (failsafeMode) {
            Autopilot::disengage();
            SpeedController::setSpeed(0);
        } else if (Autopilot::isEngaged()) {
            Autopilot::update(roll, pitch, yaw, dt_ms);
            SpeedController::setSpeed(Autopilot::getThrottleCommand());
            AngleController::setAngle(AngleController::AILERON,
                                      Autopilot::getAileronCommand());
            AngleController::setAngle(AngleController::RUDDER,
                                      Autopilot::getRudderCommand());
            AngleController::setAngle(AngleController::ELEVATOR,
                                      Autopilot::getElevatorCommand());
        } else if (controlsEnabled) {
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

        if (failsafeMode && !wasFailSafe) {
            printf("FAILSAFE = TRUE\n");
        }
        wasFailSafe = failsafeMode;
    }

    void filter_imu_data(MPU6050 &imu, MahonyFilter &filter, float dt) noexcept
    {
        assert(isInitialized_);

        if (dt < DT_LOWER_BOUND) {
            dt = DT_LOWER_BOUND;
        }
        if (dt > DT_UPPER_BOUND) {
            dt = DT_UPPER_BOUND;
        }

        MPU6050::AccelVal raw_accel = imu.getAccelValues();
        MPU6050::GyroVal raw_gyro   = imu.getGyroValues();

        float gx = raw_gyro.x * DEG_TO_RAD;
        float gy = raw_gyro.y * DEG_TO_RAD;
        float gz = raw_gyro.z * DEG_TO_RAD;

        filter.update(raw_accel.x, raw_accel.y, raw_accel.z, gx, gy, gz, dt);
        roll  = filter.getRoll();
        pitch = filter.getPitch();
        yaw   = filter.getYaw();

        DEBUG_PRINT(filter.getRoll(), filter.getPitch(), filter.getYaw(), counter);
    }

    void process_and_filter() noexcept
    {
        assert(isInitialized_);

        MPU6050 imu         = MPU6050();
        MahonyFilter filter = MahonyFilter();

        prev_filter_time  = get_absolute_time();
        prev_process_time = get_absolute_time();

        while (1) {
            absolute_time_t now = get_absolute_time();

            int64_t filter_time_diff = absolute_time_diff_us(prev_filter_time, now);
            if (filter_time_diff >= (int64_t)FILTER_FREQ_US) {
                float dt = (float)filter_time_diff / US_PER_SEC;
                filter_imu_data(imu, filter, dt);
                prev_filter_time = now;
            }

            if (absolute_time_diff_us(prev_process_time, now) >= PROCESS_FREQ_US) {
                uint32_t dt_ms
                    = (uint32_t)(absolute_time_diff_us(prev_process_time, now) / 1000);
                process_data(dt_ms);
                prev_process_time = now;
            }

            absolute_time_t next_filter_time
                = delayed_by_us(prev_filter_time, FILTER_FREQ_US);
            absolute_time_t next_process_time
                = delayed_by_us(prev_process_time, PROCESS_FREQ_US);

            absolute_time_t next_loop_start = next_filter_time;
            if (next_process_time < next_filter_time) {
                next_loop_start = next_process_time;
            }

            sleep_until(next_loop_start);
        }
    }
} // namespace FlightController

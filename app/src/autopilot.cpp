/**
 * @file autopilot.cpp
 * @brief Automatically maintains the angle and speed of the plane.
 * @author Benley Hsiang
 * @date Sep-03-2026
 */

#include "autopilot.h"

#include <cassert>
#include <cstdint>

namespace Autopilot {

    struct PID {
        float Kp {};
        float Ki {};
        float Kd {};
        float Integral {};
        float prev_error {};
        bool has_prev {false};
        static constexpr float min_output {-45.0f};
        static constexpr float max_output {45.0f};

        float update(float setpoint, float measurement, float dt_ms)
        {
            float error  = setpoint - measurement;
            float dt_sec = dt_ms / 1000.0f;

            // P
            float P = Kp * error;

            // I
            Integral += Ki * error * dt_sec;
            if (Integral > max_output) {
                Integral = max_output;
            }
            if (Integral < min_output) {
                Integral = min_output;
            }

            // D
            float D {};
            if (has_prev && dt_sec > 0.001f) {
                D = Kd * (error - prev_error) / dt_sec;
            }
            prev_error = error;
            has_prev   = true;

            // Output
            float output = P + Integral + D;
            if (output > max_output) {
                output = max_output;
            }
            if (output < min_output) {
                output = min_output;
            }

            return output;
        }

        void reset()
        {
            Integral   = 0.0f;
            prev_error = 0.0f;
            has_prev   = false;
        }
    };

    bool isInitialized_ {false};

    // Internal State
    static bool engaged_ {false};
    static float target_roll_ {};
    static float target_pitch_ {};
    static float target_yaw_ {};
    static int target_throttle_ {};
    static int aileron_cmd_ {};
    static int elevator_cmd_ {};
    static int rudder_cmd_ {};
    static int throttle_cmd_ {};
    static PID roll_pid;
    static PID pitch_pid;

    void init()
    {
        assert(!isInitialized_);

        roll_pid.Kp = 1.0f;
        roll_pid.Ki = 0.05f;
        roll_pid.Kd = 0.1f;

        pitch_pid.Kp = 1.0f;
        pitch_pid.Ki = 0.05f;
        pitch_pid.Kd = 0.1f;

        isInitialized_ = true;
    }

    void cleanup()
    {
        assert(isInitialized_);
        isInitialized_ = false;
    }

    void engage(float roll_target, float pitch_target, float yaw_target,
                int throttle_target)
    {
        assert(isInitialized_);
        engaged_ = true;

        target_roll_     = roll_target;
        target_pitch_    = pitch_target;
        target_yaw_      = yaw_target;
        target_throttle_ = throttle_target;

        roll_pid.reset();
        pitch_pid.reset();
    }

    void disengage()
    {
        assert(isInitialized_);
        engaged_ = false;
    }

    void update(float roll_current, float pitch_current, float yaw_current,
                uint32_t dt_ms)
    {
        assert(isInitialized_);
        if (!engaged_) {
            return;
        }

        float roll_output = roll_pid.update(target_roll_, roll_current, dt_ms);
        aileron_cmd_      = (int)roll_output;

        float pitch_output = pitch_pid.update(target_pitch_, pitch_current, dt_ms);
        elevator_cmd_      = (int)pitch_output;

        rudder_cmd_   = 0;
        throttle_cmd_ = target_throttle_;
    }

    [[nodiscard]] bool isEngaged()
    {
        assert(isInitialized_);
        return engaged_;
    }

    [[nodiscard]] int getAileronCommand()
    {
        assert(isInitialized_);
        return aileron_cmd_;
    }

    [[nodiscard]] int getElevatorCommand()
    {
        assert(isInitialized_);
        return elevator_cmd_;
    }

    [[nodiscard]] int getRudderCommand()
    {
        assert(isInitialized_);
        return rudder_cmd_;
    }

    [[nodiscard]] int getThrottleCommand()
    {
        assert(isInitialized_);
        return throttle_cmd_;
    }

} // namespace Autopilot

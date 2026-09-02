/**
 * @file autopilot.cpp
 * @brief Automatically maintains the angle and speed of the plane.
 * @author Benley Hsiang
 * @date Sep-02-2026
 */

#include "autopilot.h"

#include <cassert>

namespace Autopilot {
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

    void init()
    {
        assert(!isInitialized_);
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
    }

    void disengage()
    {
        assert(isInitialized_);
        engaged_ = false;
    }

    void update(float roll_current, float pitch_current, float yaw_current)
    {
        assert(isInitialized_);
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

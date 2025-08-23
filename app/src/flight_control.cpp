/**
 * @file flight_control.cpp
 * @brief Controls the direction and speed of the plane.
 * @author Benley Hsiang
 * @date Aug-22-2025
 */

#include "flight_control.h"

FlightController::FlightController() {}

FlightController::~FlightController() {}

void FlightController::changeSpeed(int percent) noexcept { speedCTRL_.setSpeed(percent); }

void FlightController::changeAngle(AngleController::ControlType controlType,
                                   int angle) noexcept
{
    angleCTRL_.setAngle(controlType, angle);
}
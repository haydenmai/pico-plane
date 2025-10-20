/**
 * @file flight_config.h
 * @brief Configurations for the plane
 * @author Hayden Mai
 * @date Oct-19-2025
 */

#ifndef FLIGHT_CONFIG_H_
#define FLIGHT_CONFIG_H_

#include "angle_control.h"
#include "hardware/uart.h"

namespace FlightConfig {
    /** @brief GPIO pin settings */
    // ESC
    constexpr int ESC_PIN {18};

    // Servos
    constexpr int AILERON_LEFT_PIN {4};
    constexpr int AILERON_RIGHT_PIN {5};
    constexpr int RUDDER_PIN {6};
    constexpr int ELEVATOR_PIN {7};

    // UART
    uart_inst_t *UART {uart1};
    constexpr int UART_TX_PIN {8};
    constexpr int UART_RX_PIN {9};


    /** @brief Maximum throttle percentage of the motors. */
    constexpr int THROTTLE_LIMIT {10};

    /** @brief Aileron servos center and range in degrees*/
    constexpr int AILERON_CTR_DEG {87};
    constexpr int AILERON_RNG_DEG {20};
    constexpr int AILERON_RIGHT_DEG_OFFSET {13};
    constexpr AngleController::TurningLimit AILERON_LIM {AILERON_CTR_DEG,
                                                         AILERON_RNG_DEG};

    /** @brief Rudder servos centre and range in degrees*/
    constexpr int RUDDER_CTR_DEG {82};
    constexpr int RUDDER_RNG_DEG {35};
    constexpr AngleController::TurningLimit RUDDER_LIM {RUDDER_CTR_DEG, RUDDER_RNG_DEG};

    /** @brief Elevator servos centre and range in degrees*/
    constexpr int ELEVATOR_CTR_DEG {95};
    constexpr int ELEVATOR_RNG_DEG {25};
    constexpr AngleController::TurningLimit ELEVATOR_LIM {ELEVATOR_CTR_DEG,
                                                          ELEVATOR_RNG_DEG};


    /** @brief CRSF Settings */
    // Channel indexes
    constexpr int AILERON_IND {0};
    constexpr int ELEVATOR_IND {1};
    constexpr int THROTTLE_IND {2};
    constexpr int RUDDER_IND {3};

    // CRSF Limits set by controller
    constexpr int CRSF_LOWER {1000};
    constexpr int CRSF_UPPER {2000};

} // namespace FlightConfig

#endif
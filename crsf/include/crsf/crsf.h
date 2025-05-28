/**
 * @file crsf.h
 * @author Hayden Mai
 * @date May-10-2025
 * @brief A class to decode CRSF protocol from a ELRS receiver.
 * @link https://github.com/tbs-fpv/tbs-crsf-spec/blob/main/crsf.md
 */

#ifndef CRSF_H_
#define CRSF_H_

#include "crsf/crc8.h"
#include "crsf/crsf_info.h"

#include <pico/stdlib.h>

#include <array>
#include <cstdint>

// TODO: Merge crsf_info.h with crsf.h (if needed)

class CRSF {
  public:
    explicit CRSF(uart_inst_t *uart, uint8_t tx_pin, uint8_t rx_pin);
    ~CRSF();

    // TODO: Configuration settings

    // TODO: Telemetry Setters (?)

    void processFrames() noexcept;

  private:
    CRC8 crc8_;
    uart_inst_t *uart_;                                  // UART number on the Pico
    std::array<uint8_t, MAX_CRSF_FRAME_SIZE> buffer_ {}; // Buffer to store frames
};

#endif
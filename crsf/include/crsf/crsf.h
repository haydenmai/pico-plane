/**
 * @file crsf.h
 * @author Hayden Mai
 * @date May-09-2025
 * @brief A class to decode CRSF protocol from a ELRS receiver.
 * @link https://github.com/tbs-fpv/tbs-crsf-spec/blob/main/crsf.md
 */

#ifndef CRSF_H_
#define CRSF_H_

#include "crsf/crsf_info.h"

#include <cstdint>
#include <pico/stdlib.h>

class CRSF {
  public:
    explicit CRSF(uart_inst_t *uart, uint8_t tx_pin, uint8_t rx_pin);
    ~CRSF();

    // TODO: Configuration settings

    // Telemetry Setters (?)

    void processFrames() noexcept;

  private:
    uart_inst_t *uart;
    void init(uint8_t tx_pin, uint8_t rx_pin) noexcept;
    void deinit();
};

#endif
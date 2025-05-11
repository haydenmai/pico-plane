/**
 * @file crsf.cpp
 * @brief A class to decode CRSF protocol from a ELRS receiver.
 * @author Hayden Mai
 * @date May-09-2025
 *
 * @link https://github.com/tbs-fpv/tbs-crsf-spec/blob/main/crsf.md
 */

#include "crsf/crsf.h"
#include "crsf/crsf_info.h"


explicit CRSF::CRSF(uart_inst_t *uart, uint8_t tx_pin, uint8_t rx_pin) : uart(uart)
{
    uart_init(uart, BAUDRATE);
    gpio_set_function(tx_pin, UART_FUNCSEL_NUM(uart, tx_pin));
    gpio_set_function(rx_pin, UART_FUNCSEL_NUM(uart, rx_pin));
}


CRSF::~CRSF() { uart_deinit(uart); }


void CRSF::processFrames() noexcept {}
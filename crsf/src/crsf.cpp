/**
 * @file crsf.cpp
 * @author Hayden Mai
 * @date May-10-2025
 * @brief A class to decode CRSF protocol from a ELRS receiver.
 *
 * This code is a modified C++ version of Britannio's pico_crsf from Github.
 *
 * @link https://github.com/tbs-fpv/tbs-crsf-spec/blob/main/crsf.md
 * @link https://github.com/britannio/pico_crsf/tree/main
 */

#include "crsf/crsf.h"
#include "crsf/crsf_info.h"


explicit CRSF::CRSF(uart_inst_t *uart, uint8_t tx_pin, uint8_t rx_pin) : uart_(uart)
{
    uart_init(uart, BAUDRATE);
    gpio_set_function(tx_pin, UART_FUNCSEL_NUM(uart, tx_pin));
    gpio_set_function(rx_pin, UART_FUNCSEL_NUM(uart, rx_pin));
}


CRSF::~CRSF() { uart_deinit(uart_); }


void CRSF::processFrames() noexcept
{
    uint idx {}, frameLen {}, crc_idx {};

    constexpr float BITS_PER_FRAME {10.0f}; // UART Frame 8 bits data, 2 bits start/stop
    constexpr float SEC_TO_US {1000000.0f};
    constexpr int US_PER_FRAME {BITS_PER_FRAME / (BAUDRATE / SEC_TO_US)
                                + 1}; // Time between UART Frames + 1 for rounding down

    while (uart_is_readable_within_us(uart_, US_PER_FRAME)) {
        buffer_[idx] = uart_getc(uart_);

        // Handle the sync byte, frame length, and type
        if (idx == 0) { // Sync byte
            // Should be the sync byte (0xC8)
            // TODO: Process different sync bytes
            if (buffer_[idx] == SYNC_BYTE) {
                idx++;
            }

        } else if (idx == 1) { // Frame length
            frameLen = buffer_[idx];
            crc_idx  = buffer_[idx] + 1; // CRC8 should be the last byte in the frame

            idx++;

            // Check if length is valid, else discard
            if (frameLen < 2 || frameLen > MAX_FRAME_LEN) {
                idx = 0;
            }
        } else if (idx == crc_idx) { // Perform Cyclic Redudancy Check (CRC)
                                     // TODO: CRC8 check here
                                     // TODO: Once checked, handle the frame based on type
        }
    }
}
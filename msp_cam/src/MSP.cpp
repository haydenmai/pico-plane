// MSP.cpp converted to Raspberry Pi Pico SDK
// Replaces Arduino Stream with Pico UART
// Note: Assumes UART0, but you can adapt as needed.

#include "msp_cam/MSP.h"
#include "hardware/uart.h"
#include "pico/stdlib.h"
#include <string.h>

#define MSP_UART_ID   uart0
#define MSP_BAUD_RATE 115200

static const uint8_t BOXIDS[30] = {
    0,  //  MSP_MODE_ARM
    1,  //  MSP_MODE_ANGLE
    2,  //  MSP_MODE_HORIZON
    3,  //  MSP_MODE_NAVALTHOLD
    5,  //  MSP_MODE_MAG
    6,  //  MSP_MODE_HEADFREE
    7,  //  MSP_MODE_HEADADJ
    8,  //  MSP_MODE_CAMSTAB
    10, //  MSP_MODE_NAVRTH
    11, //  MSP_MODE_NAVPOSHOLD
    12, //  MSP_MODE_PASSTHRU
    13, //  MSP_MODE_BEEPERON
    15, //  MSP_MODE_LEDLOW
    16, //  MSP_MODE_LLIGHTS
    19, //  MSP_MODE_OSD
    20, //  MSP_MODE_TELEMETRY
    21, //  MSP_MODE_GTUNE
    22, //  MSP_MODE_SONAR
    26, //  MSP_MODE_BLACKBOX
    27, //  MSP_MODE_FAILSAFE
    28, //  MSP_MODE_NAVWP
    29, //  MSP_MODE_AIRMODE
    30, //  MSP_MODE_HOMERESET
    31, //  MSP_MODE_GCSNAV
    32, //  MSP_MODE_HEADINGLOCK
    33, //  MSP_MODE_SURFACE
    34, //  MSP_MODE_FLAPERON
    35, //  MSP_MODE_TURNASSIST
    36, //  MSP_MODE_NAVLAUNCH
    37  //  MSP_MODE_AUTOTRIM
};


void MSP::begin(uart_inst_t *uart, uint32_t timeout)
{
    _uart    = uart;
    _timeout = timeout;
}

void MSP::reset()
{
    while (uart_is_readable(_uart)) {
        uart_getc(_uart);
    }
}

void MSP::send(uint8_t messageID, void *payload, uint8_t size)
{
    uart_putc(_uart, '$');
    uart_putc(_uart, 'M');
    uart_putc(_uart, '<');
    uart_putc(_uart, size);
    uart_putc(_uart, messageID);
    uint8_t checksum    = size ^ messageID;
    uint8_t *payloadPtr = (uint8_t *)payload;
    for (uint8_t i = 0; i < size; ++i) {
        uint8_t b = *(payloadPtr++);
        checksum ^= b;
        uart_putc(_uart, b);
    }
    uart_putc(_uart, checksum);
}

bool MSP::recv(uint8_t *messageID, void *payload, uint8_t maxSize, uint8_t *recvSize)
{
    uint32_t start = to_ms_since_boot(get_absolute_time());

    while (true) {
        while (!uart_is_readable(_uart)) {
            if (to_ms_since_boot(get_absolute_time()) - start >= _timeout)
                return false;
        }

        char header[3];
        for (int i = 0; i < 3; i++)
            header[i] = uart_getc(_uart);

        if (header[0] == '$' && header[1] == 'M'
            && (header[2] == '<' || header[2] == '>')) {
            *recvSize  = uart_getc(_uart);
            *messageID = uart_getc(_uart);

            uint8_t checksum    = *recvSize ^ *messageID;
            uint8_t *payloadPtr = (uint8_t *)payload;
            uint8_t idx         = 0;
            while (idx < *recvSize) {
                if (to_ms_since_boot(get_absolute_time()) - start >= _timeout)
                    return false;
                if (uart_is_readable(_uart)) {
                    uint8_t b = uart_getc(_uart);
                    checksum ^= b;
                    if (idx < maxSize)
                        *(payloadPtr++) = b;
                    ++idx;
                }
            }
            for (; idx < maxSize; ++idx)
                *(payloadPtr++) = 0;

            while (!uart_is_readable(_uart)) {
                if (to_ms_since_boot(get_absolute_time()) - start >= _timeout)
                    return false;
            }
            uint8_t recvChecksum = uart_getc(_uart);
            return recvChecksum == checksum;
        }
    }
}

bool MSP::activityDetected()
{
    uint32_t start = to_ms_since_boot(get_absolute_time());
    while (!uart_is_readable(_uart)) {
        if (to_ms_since_boot(get_absolute_time()) - start >= _timeout)
            return false;
    }
    return true;
}

bool MSP::waitFor(uint8_t messageID, void *payload, uint8_t maxSize, uint8_t *recvSize)
{
    uint8_t recvID;
    uint8_t dummySize;
    uint32_t start = to_ms_since_boot(get_absolute_time());

    while (to_ms_since_boot(get_absolute_time()) - start < _timeout) {
        if (recv(&recvID, payload, maxSize, recvSize ? recvSize : &dummySize)) {
            if (recvID == messageID)
                return true;
        }
    }
    return false;
}

bool MSP::request(uint8_t messageID, void *payload, uint8_t maxSize, uint8_t *recvSize)
{
    send(messageID, NULL, 0);
    return waitFor(messageID, payload, maxSize, recvSize);
}

bool MSP::command(uint8_t messageID, void *payload, uint8_t size, bool waitACK)
{
    send(messageID, payload, size);
    if (waitACK)
        return waitFor(messageID, NULL, 0);
    return true;
}

bool MSP::getActiveModes(uint32_t *activeModes)
{
    msp_status_t status;
    if (request(MSP_STATUS, &status, sizeof(status))) {
        uint8_t ids[30];
        uint8_t recvSize;
        if (request(MSP_BOXIDS, ids, sizeof(ids), &recvSize)) {
            *activeModes = 0;
            for (uint8_t i = 0; i < recvSize; ++i) {
                if (status.flightModeFlags & (1 << i)) {
                    for (uint8_t j = 0; j < sizeof(BOXIDS); ++j) {
                        if (BOXIDS[j] == ids[i]) {
                            *activeModes |= 1 << j;
                            break;
                        }
                    }
                }
            }
            return true;
        }
    }
    return false;
}

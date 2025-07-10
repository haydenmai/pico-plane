#ifndef MSP_H
#define MSP_H

#include <stdint.h>
#include "pico/stdlib.h"
#include "hardware/uart.h"

// Adjust these as needed for your hardware setup
#define MSP_UART         uart0
#define MSP_UART_TX_PIN  0
#define MSP_UART_RX_PIN  1
#define MSP_UART_BAUD    115200

// Example MSP message types (you can extend this)
#define MSP_STATUS   101
#define MSP_BOXIDS   113

// Example status struct matching MSP_STATUS payload
typedef struct __attribute__((packed)) {
    uint16_t cycleTime;
    uint16_t i2cErrorCount;
    uint16_t sensor;
    uint32_t flightModeFlags;
    uint8_t  configProfileIndex;
} msp_status_t;

class MSP {
public:
    void begin(uart_inst_t *uart, uint32_t timeout_ms = 1000);
    void reset();

    void send(uint8_t messageID, void* payload, uint8_t size);
    bool recv(uint8_t* messageID, void* payload, uint8_t maxSize, uint8_t* recvSize);
    bool activityDetected();

    bool waitFor(uint8_t messageID, void* payload, uint8_t maxSize, uint8_t* recvSize = nullptr);
    bool request(uint8_t messageID, void* payload, uint8_t maxSize, uint8_t* recvSize = nullptr);
    bool command(uint8_t messageID, void* payload, uint8_t size, bool waitACK = true);

    bool getActiveModes(uint32_t* activeModes);

private:
	uart_inst_t *_uart;
    uint32_t _timeout;
    int readByteBlocking(uint32_t timeout_ms);
};

#endif // MSP_H

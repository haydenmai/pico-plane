#include "hardware/adc.h"
#include "hardware/uart.h"
#include "pico/stdlib.h"
#include <cstring>
#include "pico/cyw43_arch.h"

// Constants
#define UART_ID     uart0
#define UART_TX_PIN 17
#define UART_RX_PIN 16

#define ANALOG_IN    26 // ADC0 on Pico GPIO26 corresponds to A0
#define VOLT_DIVIDER 48

// Timing constants
#define UNARMED_MILLIS  3000
#define MSP_INTERVAL_MS 100

#define MSP_SET_FLIGHT_MODE 214  // Replace with the actual ID used by DJI, if different

// Globals for timing
absolute_time_t previous_time;
absolute_time_t activity_detected_time;
bool activity_detected        = false;
bool light_on                 = true;
uint32_t flight_mode_flags    = 0x00000002;
uint32_t custom_mode          = 0;
uint32_t previous_flight_mode = 0;
uint8_t vbat                  = 0;
uint8_t battery_cell_count    = 3;
uint32_t general_counter      = MSP_INTERVAL_MS;

// Placeholder for MSP structures and methods
// You will need to port MSP library calls yourself or adapt them
void msp_send(const char *msg)
{
    // Stub: Send MSP message over UART
}

bool msp_activity_detected()
{
    // Stub: Implement detecting activity on UART line
    return true;
}

void send_osd_config()
{
    // Stub: send OSD config message
}

void send_msp_to_airunit() {
    const uint8_t command = MSP_SET_FLIGHT_MODE;

    // Send 4 bytes for flight_mode_flags (little-endian)
    uint8_t payload[4];
    payload[0] = flight_mode_flags & 0xFF;
    payload[1] = (flight_mode_flags >> 8) & 0xFF;
    payload[2] = (flight_mode_flags >> 16) & 0xFF;
    payload[3] = (flight_mode_flags >> 24) & 0xFF;

    uint8_t payload_size = sizeof(payload);

    // Build MSP frame
    uint8_t frame[6 + payload_size];
    uint8_t idx = 0;

    frame[idx++] = '$';
    frame[idx++] = 'M';
    frame[idx++] = '>';

    frame[idx++] = payload_size;
    frame[idx++] = command;

    // Add payload
    for (int i = 0; i < payload_size; i++) {
        frame[idx++] = payload[i];
    }

    // Calculate checksum
    uint8_t checksum = payload_size ^ command;
    for (int i = 0; i < payload_size; i++) {
        checksum ^= payload[i];
    }
    frame[idx++] = checksum;

    // Send frame via UART
    for (int i = 0; i < idx; i++) {
        uart_putc(UART_ID, frame[i]);
    }
}


void set_flight_mode_flags(bool arm)
{
    if ((flight_mode_flags == 0x00000002) && arm) {
        flight_mode_flags = 0x00000003; // arm
    } else if ((flight_mode_flags == 0x00000003) && !arm) {
        flight_mode_flags = 0x00000002; // disarm
    }
}

void display_flight_mode()
{
    // Stub: Display flight mode text
}

void get_voltage_sample()
{
    uint16_t raw = adc_read(); // 12-bit value from ADC
    vbat         = raw * 10 / VOLT_DIVIDER;
}

void set_battery_cells_number()
{
    if (vbat < 43)
        battery_cell_count = 1;
    else if (vbat < 85)
        battery_cell_count = 2;
    else if (vbat < 127)
        battery_cell_count = 3;
    else if (vbat < 169)
        battery_cell_count = 4;
    else if (vbat < 211)
        battery_cell_count = 5;
    else if (vbat < 255)
        battery_cell_count = 6;
}

int main()
{
    stdio_init_all();
	cyw43_arch_init();

    // Init UART1 for MSP communication
    uart_init(UART_ID, 115200);
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);

    // Init ADC on ANALOG_IN pin (GPIO26)
    adc_init();
    adc_gpio_init(ANALOG_IN);
    adc_select_input(0);

    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);

    // Delay to simulate Arduino delay(1000)
    sleep_ms(1000);

    previous_time = get_absolute_time();

    while (true) {
        if (!activity_detected) {
            cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);

            // Wait for Air Unit activity
            while (!msp_activity_detected()) {
                tight_loop_contents(); // busy wait
            }
            activity_detected      = true;
            activity_detected_time = get_absolute_time();
        }

        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);

        absolute_time_t current_time = get_absolute_time();
        int64_t elapsed_ms = absolute_time_diff_us(previous_time, current_time) / 1000;

        if (elapsed_ms >= MSP_INTERVAL_MS) {
            previous_time = current_time;

            if (general_counter % 300 == 0) {
                get_voltage_sample();
            	cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, light_on ? 0 : 1);
                light_on = !light_on;
            }

            int64_t time_since_activity
                = absolute_time_diff_us(activity_detected_time, current_time) / 1000;

            if (time_since_activity < UNARMED_MILLIS) {
                set_flight_mode_flags(false);
            } else {
                set_flight_mode_flags(true);
            }

            send_msp_to_airunit();
            general_counter += MSP_INTERVAL_MS;
        }

        if (custom_mode != previous_flight_mode) {
            previous_flight_mode = custom_mode;
            display_flight_mode();
        }

        if (battery_cell_count == 0 && vbat > 0) {
            set_battery_cells_number();
        }

        if (general_counter % 10000 == 0) {
            display_flight_mode();
        }
    }
}

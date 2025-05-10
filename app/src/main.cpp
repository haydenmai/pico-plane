/**
 * @file main.cpp
 *
 * @author Hayden Mai
 * @brief Controls an airplane and data
 * @date May-07-2025
 */


#include "hardware/adc.h"
#include "pico/cyw43_arch.h"
#include "pico/stdlib.h"

#include "hal/pico_led.h"
#include "hal/servo_mg90s.h"

#include <iostream>

int main()
{
    stdio_init_all();
    cyw43_arch_init();
    adc_init();

    // 1) Init UART0 @ 115200 baud for our “print” port
    uart_init(uart0, 115200);
    gpio_set_function(0, GPIO_FUNC_UART); // UART0 TX → GP0
    gpio_set_function(1, GPIO_FUNC_UART); // UART0 RX → GP1  (not used here)

    // UART Setup
    uart_init(uart1, 420000);
    gpio_set_function(8, UART_FUNCSEL_NUM(uart1, 8));
    gpio_set_function(9, UART_FUNCSEL_NUM(uart1, 9));

    // auto &onboard_led = PicoLED::get();
    auto servo_16 = ServoMG90S(16);
    auto servo_17 = ServoMG90S(17);


    while (true) {
        if (uart_is_readable(uart1)) {
            uint8_t b = uart_getc(uart1);
            uart_putc(uart0, b);
        }
        /*
            onboard_led.on();
sleep_ms(100);
onboard_led.off();
sleep_ms(2000);
        */
    }

    stdio_deinit_all();
    cyw43_arch_deinit();
    return 0;
}

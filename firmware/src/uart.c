#include "uart.h"

#include <hardware/uart.h>
#include <pico/stdio.h>
#include <hardware/gpio.h>

#include "variables.h"
#include "output.h"

#include <stdio.h>

static void uart_connect_()
{
    uart_init(uart0, var.uart.baud);
    gpio_set_function(0, GPIO_FUNC_UART);
    gpio_set_function(1, GPIO_FUNC_UART);
    uart_set_hw_flow(uart0, var.uart.hflow, var.uart.hflow);
    uart_set_format(uart0, var.uart.mode[0] - '0', var.uart.mode[2] - '0',
                    var.uart.mode[1] == 'n' ? UART_PARITY_NONE : (
                            var.uart.mode[1] == 'e' ? UART_PARITY_EVEN : UART_PARITY_ODD));
    output_set_mode(var.uart.output);

    output_print("RX0 ", C_INPUT);
    output_print("TX ", C_OUTPUT);
    output_print("RX1\n", C_SNIFF);

    // TODO - setup interrupt
}

static void uart_disconnect_()
{
}

void uart_init_()    // make this generic?
{
    uart_connect_();

    for (;;) {
        output_print_queues();
        int c = getchar_timeout_us(0);
        if (c != PICO_ERROR_TIMEOUT) {
            if (c == 0x3) {  // CTRL+C
                output_print("\n", C_NONE);
                break;
            }
            output_queue_add(c, C_OUTPUT);
            uart_putc_raw(uart0, c);
            if (c == 0xd) {
                output_queue_add(0xa, C_OUTPUT);
                uart_putc_raw(uart0, 0xa);
            }
        }
    }

    uart_disconnect_();
}
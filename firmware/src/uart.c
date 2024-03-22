#include "uart.h"

#include <hardware/uart.h>
#include <pico/stdio.h>
#include <hardware/gpio.h>

#include "variables.h"
#include "output.h"

static void on_uart_rx(uart_inst_t* uart, Channel channel)
{
    while (uart_is_readable(uart)) {
        char c = uart_getc(uart);
        output_queue_add(c, channel);
        if (c == 0xd)
            output_queue_add(0xa, channel);
    }
}

static void on_uart0_rx()
{
    on_uart_rx(uart0, C_INPUT);
}

static void on_uart1_rx()
{
    on_uart_rx(uart1, C_SNIFF);
}

static void uart_connect_()
{
    uart_inst_t* uarts[2] = { uart0, uart1 };

    // setup uart
    for (size_t i = 0; i < 2; i++) {
        uart_init(uarts[i], var.uart.baud);
        uart_set_hw_flow(uarts[i], var.uart.hflow, var.uart.hflow);
        uart_set_format(uarts[i],
                        var.uart.mode[0] - '0',
                        var.uart.mode[2] - '0',
                        var.uart.mode[1] == 'n' ? UART_PARITY_NONE : (var.uart.mode[1] == 'e' ? UART_PARITY_EVEN : UART_PARITY_ODD));
    }
    gpio_set_function(0, GPIO_FUNC_UART);
    gpio_set_function(1, GPIO_FUNC_UART);
    gpio_set_function(5, GPIO_FUNC_UART);
    output_set_mode(var.uart.output);

    // print header
    output_print("RX0 ", C_INPUT);
    output_print("TX ", C_OUTPUT);
    output_print("RX1\n", C_SNIFF);

    // setup interrupt
    irq_set_exclusive_handler(UART0_IRQ, on_uart0_rx);
    irq_set_enabled(UART0_IRQ, true);
    uart_set_irq_enables(uart0, true, false);
    irq_set_exclusive_handler(UART1_IRQ, on_uart1_rx);
    irq_set_enabled(UART1_IRQ, true);
    uart_set_irq_enables(uart1, true, false);
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


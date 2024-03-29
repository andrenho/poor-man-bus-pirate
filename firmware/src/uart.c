#include "uart.h"

#include <hardware/uart.h>
#include <pico/stdio.h>
#include <hardware/gpio.h>
#include <stdio.h>
#include <pico/time.h>

#include "variables.h"
#include "output.h"
#include "pins.h"

#define TX_PIN  0
#define RX0_PIN 1
#define RX1_PIN 5
#define GUESS_THRESHOLD 10

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

static void uart_setup_(uart_inst_t* uart)
{
    uart_init(uart, var.uart.baud);
    uart_set_hw_flow(uart, var.uart.hflow, var.uart.hflow);
    uart_set_format(uart,
                    var.uart.mode[0] - '0',
                    var.uart.mode[2] - '0',
                    var.uart.mode[1] == 'n' ? UART_PARITY_NONE : (var.uart.mode[1] == 'e' ? UART_PARITY_EVEN : UART_PARITY_ODD));
}

static void uart_connect_()
{
    // setup uart
    uart_setup_(uart0);
    uart_setup_(uart1);
    gpio_set_function(TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(RX0_PIN, GPIO_FUNC_UART);
    gpio_set_function(RX1_PIN, GPIO_FUNC_UART);
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
    irq_set_enabled(UART0_IRQ, false);
    irq_set_enabled(UART1_IRQ, false);
    uart_deinit(uart0);
    uart_deinit(uart1);
    pins_reset();
}

void uart_init_()    // make this generic?
{
    uart_connect_();

    for (;;) {
        output_print_queues();
        int c = output_get_char();
        if (c == CTRL_C) {
            output_print("\n", C_NONE);
            break;
        } else if (c != NO_CHAR) {
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

void uart_guess_speed()
{
    uart_setup_(uart0);
    gpio_set_function(TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(RX0_PIN, GPIO_FUNC_UART);

    static const size_t speeds[] = {
            3000000, 2000000, 1000000, 500000, 250000, 230400, 115200,
            57600, 38400, 19200, 14400, 9600, 4800, 2400, 1200, 600, 300, 0
    };

    for (size_t i = 0; speeds[i]; ++i) {
        printf("%d... ", speeds[i]);
        uart_set_baudrate(uart0, speeds[i]);
        sleep_ms(500);

        size_t j = 0;
        while (uart_is_readable(uart0)) {
            char c = uart_getc(uart0);
            if (c >= 32 && c < 127 && c != '?')
                ++j;
        }
        if (j >= GUESS_THRESHOLD) {
            printf("found!\n");
            return;
        } else {
            printf("no\n");
        }
    }
}
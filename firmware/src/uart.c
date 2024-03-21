#include "uart.h"

#include <hardware/uart.h>

#include "variables.h"
#include "output.h"

static void uart_connect_()
{
    uart_init(uart0, var.uart.baud);
    uart_set_hw_flow(uart0, var.uart.hflow, var.uart.hflow);
    uart_set_format(uart0, var.uart.mode[0] - '0', var.uart.mode[2] - '0',
                    var.uart.mode[1] == 'N' ? UART_PARITY_NONE : (
                            var.uart.mode[1] == 'E' ? UART_PARITY_EVEN : UART_PARITY_ODD));
    output_set_mode(var.uart.output);

    output_print("RX0 ", C_INPUT);
    output_print("TX ", C_OUTPUT);
    output_print("RX1\n", C_SNIFF);
}

void uart_init_()
{
    uart_connect_();
}
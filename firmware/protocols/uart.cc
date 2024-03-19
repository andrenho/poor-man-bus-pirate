#include "uart.hh"

#include <hardware/gpio.h>

UART::UART(uart_inst_t *uart_id)
    : uart_id_(uart_id),
      tx_pin_(uart_id_ == uart0 ? 0 : 16),
      rx_pin_(uart_id_ == uart0 ? 1 : 17)
{
    gpio_init(tx_pin_);
    gpio_init(rx_pin_);

    disable();
}

void UART::disable()
{
    uart_deinit(uart_id_);
    gpio_set_function(tx_pin_, GPIO_FUNC_NULL);
    gpio_set_function(tx_pin_, GPIO_FUNC_NULL);
    gpio_set_dir(tx_pin_, false);
    gpio_set_dir(rx_pin_, false);
    gpio_set_pulls(tx_pin_, true, false);
    gpio_set_pulls(rx_pin_, true, false);
}

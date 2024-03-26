#include "spi_master.h"

#include <hardware/spi.h>
#include <hardware/gpio.h>

#include "output.h"
#include "pins.h"

#define RX_PIN   0
#define CS_PIN   1
#define SCK_PIN  2
#define TX_PIN   3

static void spi_master_connect()
{
    // setup
    spi_init(spi0, var.spi.baud);
    spi_set_format(spi0, 8, var.spi.cpol, var.spi.cpha,
                   var.spi.order == MSB ? SPI_MSB_FIRST : SPI_LSB_FIRST);
    output_set_mode(var.spi.output);

    // setup pins
    gpio_set_function(RX_PIN, GPIO_FUNC_SPI);
    gpio_set_function(TX_PIN, GPIO_FUNC_SPI);
    gpio_set_function(SCK_PIN, GPIO_FUNC_SPI);
    gpio_set_function(CS_PIN, GPIO_FUNC_SPI);
}

static void spi_master_disconnect()
{
    spi_deinit(spi0);
    pins_reset();
}

void spi_master_init()
{
    spi_master_connect();

    // print header
    output_print("MOSI ", C_INPUT);
    output_print("MISO\n", C_OUTPUT);

    for (;;) {
        output_print_queues();
        int c = output_get_char();
        if (c == CTRL_C) {
            output_print("\n", C_NONE);
            break;
        } else if (c != NO_CHAR) {
            output_queue_add(c, C_OUTPUT);
            uint8_t response;
            spi_write_read_blocking(spi0, (uint8_t *) &c, &response, 1);
            output_queue_add(response, C_INPUT);
        }
    }

    spi_master_disconnect();
}

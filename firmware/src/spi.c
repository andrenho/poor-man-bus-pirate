#include "spi.h"

#include <hardware/spi.h>
#include <hardware/gpio.h>
#include <pico/stdio.h>
#include <stdio.h>

#include "output.h"

#define RX0_PIN  0
#define RX1_PIN  8
#define TX_PIN   3
#define SCK_PIN  2
#define CS_PIN   1

static volatile char next_char = 0x0;

static void on_spi0_xchg()
{
    uint8_t c;
    while (spi_is_readable(spi0)) {
        output_queue_add(next_char, C_OUTPUT);
        spi_read_blocking(spi0, next_char, &c, 1);
        next_char = var.spi.autorespond;
        output_queue_add(c, C_INPUT);
    }
}

static void spi_slave_connect()
{
    if (var.spi.cpha == 0 && var.spi.cpol == 0)
        printf("Warning: due to a RPi Pico limitation, if CPHA=0 and CPOL=0, CS line must be asserted high between bytes!\n");

    // setup SPI
    spi_init(spi0, var.spi.baud);
    spi_set_slave(spi0, true);
    spi_set_format(spi0, 8, var.spi.cpol, var.spi.cpha,
                   var.spi.order == MSB ? SPI_MSB_FIRST : SPI_LSB_FIRST);
    gpio_set_function(RX0_PIN, GPIO_FUNC_SPI);
    gpio_set_function(TX_PIN, GPIO_FUNC_SPI);
    gpio_set_function(SCK_PIN, GPIO_FUNC_SPI);
    gpio_set_function(CS_PIN, GPIO_FUNC_SPI);
    output_set_mode(var.spi.output);

    // print header
    output_print("MOSI ", C_INPUT);
    output_print("MISO\n", C_OUTPUT);

    // Enable the RX FIFO interrupt (RXIM)
    spi0_hw->imsc = 1 << 2;

    // setup interrupt
    irq_set_enabled(SPI0_IRQ, true);
    irq_set_exclusive_handler(SPI0_IRQ, on_spi0_xchg);
}

static void spi_disconnect()
{
    irq_set_enabled(SPI0_IRQ, false);
    irq_set_enabled(SPI1_IRQ, false);
    spi_deinit(spi0);
    spi_deinit(spi1);
    gpio_set_function(RX0_PIN, GPIO_FUNC_NULL);
    gpio_set_function(RX1_PIN, GPIO_FUNC_NULL);
    gpio_set_function(TX_PIN, GPIO_FUNC_NULL);
    gpio_set_function(SCK_PIN, GPIO_FUNC_NULL);
    gpio_set_function(CS_PIN, GPIO_FUNC_NULL);
    gpio_set_input_enabled(RX0_PIN, true);
    gpio_set_input_enabled(RX1_PIN, true);
    gpio_set_input_enabled(TX_PIN, true);
    gpio_set_input_enabled(SCK_PIN, true);
    gpio_set_input_enabled(CS_PIN, true);
}

void spi_slave_init()
{
    spi_slave_connect();

    next_char = var.spi.autorespond;

    for (;;) {
        output_print_queues();
        int c = getchar_timeout_us(0);
        if (c == 0x3) {  // CTRL+C
            output_print("\n", C_NONE);
            break;
        } else if (c != PICO_ERROR_TIMEOUT) {
            next_char = c;
        }
    }

    spi_disconnect();
}
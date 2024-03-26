#include "spi_slave.h"

#include <hardware/spi.h>
#include <hardware/gpio.h>
#include <pico/stdio.h>
#include <stdio.h>

#include "output.h"
#include "pins.h"

#define RX0_PIN   0
#define CS0_PIN   1
#define SCK0_PIN  2
#define TX_PIN    3

#define RX1_PIN   8
#define CS1_PIN   9
#define SCK1_PIN 10

static volatile char next_char = 0x0;
static volatile bool print_output = true;

static void on_spi0_xchg()
{
    uint8_t c;
    while (spi_is_readable(spi0)) {
        if (print_output)
            output_queue_add(next_char, C_OUTPUT);
        spi_read_blocking(spi0, next_char, &c, 1);
        next_char = var.spi.autorespond;
        output_queue_add(c, C_INPUT);
    }
}

static void on_spi1_xchg()
{
    uint8_t c;
    while (spi_is_readable(spi1)) {
        spi_read_blocking(spi1, var.spi.autorespond, &c, 1);  // what we're sending doesn't matter, as TX1 is disconnected
        output_queue_add(c, C_SNIFF);
    }
}

static void spi_slave_setup(spi_inst_t* spi)
{
    spi_init(spi, var.spi.baud);
    spi_set_slave(spi, true);
    spi_set_format(spi, 8, var.spi.cpol, var.spi.cpha,
                   var.spi.order == MSB ? SPI_MSB_FIRST : SPI_LSB_FIRST);
}

static void spi_slave_connect()
{
    if (var.spi.cpha == 0 && var.spi.cpol == 0)
        printf("Warning: due to a RPi Pico limitation, if CPHA=0 and CPOL=0, CS line must be asserted high between bytes!\n");

    // setup SPI
    spi_slave_setup(spi0);
    gpio_set_function(RX0_PIN, GPIO_FUNC_SPI);
    gpio_set_function(TX_PIN, GPIO_FUNC_SPI);
    gpio_set_function(SCK0_PIN, GPIO_FUNC_SPI);
    gpio_set_function(CS0_PIN, GPIO_FUNC_SPI);
    output_set_mode(var.spi.output);

    // Enable the RX FIFO interrupt (RXIM)
    spi0_hw->imsc = 1 << 2;

    // setup interrupt
    irq_set_enabled(SPI0_IRQ, true);
    irq_set_exclusive_handler(SPI0_IRQ, on_spi0_xchg);
}

static void spi_sniff_connect()
{
    spi_slave_setup(spi1);

    // setup SPI
    gpio_set_function(RX1_PIN, GPIO_FUNC_SPI);
    gpio_set_function(SCK1_PIN, GPIO_FUNC_SPI);
    gpio_set_function(CS1_PIN, GPIO_FUNC_SPI);

    // Enable the RX FIFO interrupt (RXIM)
    spi1_hw->imsc = 1 << 2;

    // setup interrupt
    irq_set_enabled(SPI1_IRQ, true);
    irq_set_exclusive_handler(SPI1_IRQ, on_spi1_xchg);
}

static void spi_disconnect()
{
    irq_set_enabled(SPI0_IRQ, false);
    irq_set_enabled(SPI1_IRQ, false);
    spi_deinit(spi0);
    spi_deinit(spi1);
    pins_reset();
}

void spi_slave_init(bool sniff)
{
    print_output = !sniff;

    spi_slave_connect();
    if (sniff)
        spi_sniff_connect();

    // print header
    output_print("MOSI0 ", C_INPUT);
    if (!sniff)
        output_print("MISO\n", C_OUTPUT);
    else
        output_print("MOSI1\n", C_SNIFF);

    next_char = var.spi.autorespond;

    for (;;) {
        output_print_queues();
        int c = output_get_char();
        if (c == CTRL_C) {
            output_print("\n", C_NONE);
            break;
        } else if (c != NO_CHAR) {
            next_char = c;
        }
    }

    spi_disconnect();
}
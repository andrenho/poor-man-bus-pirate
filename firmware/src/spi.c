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
    output_queue_add('X', C_INPUT);
}

static void spi_slave_connect()
{
    // setup SPI
    spi_init(spi0, var.spi.baud);
    spi_set_slave(spi0, true);
    spi_set_format(spi0, 8, var.spi.cpol, var.spi.cpha,
                   var.spi.order == MSB ? SPI_MSB_FIRST : SPI_LSB_FIRST);
    gpio_set_function(RX0_PIN, GPIO_FUNC_SPI);
    gpio_set_function(TX_PIN, GPIO_FUNC_SPI);
    gpio_set_function(SCK_PIN, GPIO_FUNC_SPI);
    gpio_set_function(CS_PIN, GPIO_FUNC_SPI);

    // print header
    output_print("MOSI ", C_INPUT);
    output_print("MISO\n", C_OUTPUT);

    // setup interrupt
    /*
    irq_set_exclusive_handler(SPI0_IRQ, on_spi0_xchg);
    irq_set_enabled(SPI0_IRQ, true);
     */
}

static void spi_disconnect()
{
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
        } else if (c != 0) {
            next_char = c;
        }

        uint8_t buf1, buf2;
        spi_write_read_blocking(spi0, &buf1, &buf2, 1);
        printf("%x\n", buf2);
    }

    spi_disconnect();
}
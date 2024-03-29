#include "i2c_master.h"

#include <hardware/i2c.h>
#include <hardware/gpio.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

#include "variables.h"
#include "pins.h"
#include "output.h"
#include "util/getline.h"

#define SDA_PIN 0
#define SCL_PIN 1

static void i2c_master_connect()
{
    i2c_init(i2c0, var.i2c.baud);
    gpio_set_function(SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(SDA_PIN);
    gpio_pull_up(SCL_PIN);

    output_set_mode(var.i2c.output);
}

static void i2c_master_disconnect()
{
    i2c_deinit(i2c0);
    pins_reset();
}

static bool reserved_addr(uint8_t addr) {
    return (addr & 0x78) == 0 || (addr & 0x78) == 0x78;
}

void i2c_master_scan()
{
    i2c_master_connect();

    output_print("", C_NONE);
    printf("   0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F\n");

    for (int addr = 0; addr < (1 << 7); ++addr) {
        if (addr % 16 == 0) {
            printf("%02x ", addr);
        }

        // Perform a 1-byte dummy read from the probe address. If a slave
        // acknowledges this address, the function returns the number of bytes
        // transferred. If the address byte is ignored, the function returns
        // -1.

        // Skip over any reserved addresses.
        int ret;
        uint8_t rxdata;
        if (reserved_addr(addr))
            ret = PICO_ERROR_GENERIC;
        else
            ret = i2c_read_blocking(i2c0, addr, &rxdata, 1, false);

        printf(ret < 0 ? "." : "@");
        printf(addr % 16 == 15 ? "\n" : "  ");
    }

    i2c_master_disconnect();
}

static size_t get_communication_parameters(char* operation, char* stop, uint8_t bytes_to_write[200], unsigned int* bytes_to_read)
{
    do {
        printf("Operation ([r]ead / [w]rite / [q]uit)? ");
        *operation = getchar();
        printf("%c\n", *operation);
    } while (*operation != 'w' && *operation != 'r' && *operation != 'q');

    if (*operation == 'q')
        return 0;

    do {
        printf("End with [s]top or [r]epeated stop? ");
        *stop = getchar();
        printf("%c\n", *stop);
    } while (*stop != 's' && *stop != 'r');

    size_t n_bytes_write = 0;
    char bytes[1024];

    if (*operation == 'w') {
        printf("Bytes to write (space separated)? ");
        getline(bytes, sizeof bytes);

        char* token = strtok(bytes, " ");
        while (token != NULL) {
            bytes_to_write[n_bytes_write++] = strtol(token, NULL, 16);
            token = strtok(NULL, " ");
        }
    }

    printf("Number of bytes to read? ");
    getline(bytes, sizeof bytes);
    do {
        *bytes_to_read = strtoul(bytes, NULL, 10);
    } while (*bytes_to_read == ULONG_MAX);

    return n_bytes_write;
}

void i2c_master_init(uint8_t addr)
{
    i2c_master_connect();

    output_print("MASTER ", C_OUTPUT);
    output_print("SLAVE\n", C_INPUT);

    for (;;) {

        output_print("\n", C_NONE);

        char operation;
        char stop;
        uint8_t bytes_to_write[200];
        unsigned int n_bytes_to_read;
        size_t n_bytes_to_write = get_communication_parameters(&operation, &stop, bytes_to_write, &n_bytes_to_read);

        uint8_t bytes_to_read[n_bytes_to_read];

        switch (operation) {
            case 'q':
                goto done;
            case 'w':
                output_queue_add(addr << 1, C_OUTPUT);
                for (size_t i = 0; i < n_bytes_to_write; ++i)
                    output_queue_add(bytes_to_write[i], C_OUTPUT);
                i2c_write_blocking(i2c0, addr, bytes_to_write, n_bytes_to_write, stop == 'r');
                output_print_queues();

                if (bytes_to_read > 0) {
                    output_queue_add(addr << 1, C_INPUT);
                    i2c_read_blocking(i2c0, addr, bytes_to_read, n_bytes_to_read, false);
                    for (size_t i = 0; i < n_bytes_to_read; ++i)
                        output_queue_add(bytes_to_read[i], C_OUTPUT);
                    output_print_queues();
                }
                break;

            case 'r':
                output_queue_add(addr << 1, C_OUTPUT);
                output_print_queues();
                i2c_read_blocking(i2c0, addr, bytes_to_read, n_bytes_to_read, false);
                for (size_t i = 0; i < n_bytes_to_read; ++i)
                    output_queue_add(bytes_to_read[i], C_OUTPUT);
                output_print_queues();
                break;
        }
    }

done:
    i2c_master_disconnect();
}
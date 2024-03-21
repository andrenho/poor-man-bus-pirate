#include "variables.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <setjmp.h>
#include <limits.h>
#include <hardware/flash.h>
#include <hardware/sync.h>

#include "command.h"

#define FLASH_TARGET_OFFSET (3 * 512 * 1024)  // top quadrant of memory

Config var;

static jmp_buf variable_error;

static void variables_save()
{
    /*
    for (int i = 0; i < sizeof var; ++i)
        printf("%02X ", ((uint8_t const *) &var)[i]);
    printf("\n");
     */

    uint32_t ints = save_and_disable_interrupts();
    flash_range_erase(FLASH_TARGET_OFFSET, sizeof(Config));
    flash_range_program(FLASH_TARGET_OFFSET, (uint8_t const *) &var, sizeof(Config));
    restore_interrupts(ints);
}

void variables_load()
{
    char* p = (char *) (XIP_BASE + FLASH_TARGET_OFFSET);
    memcpy(&var, p, sizeof(Config));

    /*
    for (int i = 0; i < sizeof var; ++i)
        printf("%02X ", p[i]);
    printf("\n");
     */
}

void variables_reset()
{
    var.uart.baud = 115200;
    strcpy(var.uart.mode, "8n1");
    var.uart.hflow = false;
    var.uart.output = ASCII;

    var.spi.cpol = 0;
    var.spi.cpha = 0;
    var.spi.order = MSB;
    var.spi.baud = 100000;
    var.spi.autorespond = false;
    var.spi.output = HEX;

    var.i2c.baud = 100000;
    var.i2c.output = HEX;

    variables_save();
}

static uint32_t to_uint(const char* value)
{
    unsigned long v;
    if (value[0] == '0' && value[1] == 'x')
        v = strtoul(&value[2], NULL, 16);
    else
        v = strtoul(value, NULL, 10);
    if (v == ULONG_MAX)
        longjmp(variable_error, 1);
    return v;
}

static uint8_t to_byte(const char* value)
{
    return (uint8_t) to_uint(value);
}

static bool to_bool(const char* value)
{
    if (strcmp(value, "true") == 0 || strcmp(value, "on") == 0 || strcmp(value, "1") == 0)
        return true;
    else if (strcmp(value, "false") == 0 || strcmp(value, "off") == 0 || strcmp(value, "0") == 0)
        return false;
    else
        longjmp(variable_error, 1);
}

static Output to_output(const char* value)
{
    if (strcmp(value, "ascii") == 0)
        return ASCII;
    else if (strcmp(value, "dec") == 0)
        return DEC;
    else if (strcmp(value, "bin") == 0)
        return BIN;
    else if (strcmp(value, "hex") == 0)
        return HEX;
    else
        longjmp(variable_error, 1);
}

static Order to_order(const char* value)
{
    if (strcmp(value, "lsb") == 0)
        return LSB;
    else if (strcmp(value, "msb") == 0)
        return MSB;
    else
        longjmp(variable_error, 1);
}

static void uart_mode(const char* value, char* mode)
{
    if ((value[0] != '8' && value[0] != '7')
    ||  (value[1] != 'n' && value[1] != 'e' && value[1] != 'o')
    ||  (value[2] != '0' && value[2] != '1')
    ||  value[3] != '\0')
        longjmp(variable_error, 1);
    strcpy(mode, value);
}

void variable_set(const char* key, const char* value)
{
    if (!setjmp(variable_error)) {
        if (strcmp(key, "uart.baud") == 0) {
            var.uart.baud = to_uint(value);
        } else if (strcmp(key, "uart.mode") == 0) {
            uart_mode(value, var.uart.mode);
        } else if (strcmp(key, "uart.hflow") == 0) {
            var.uart.hflow = to_bool(value);
        } else if (strcmp(key, "uart.output") == 0) {
            var.uart.output = to_output(value);
        } else if (strcmp(key, "spi.cpol") == 0) {
            var.spi.cpol = to_bool(value);
        } else if (strcmp(key, "spi.cpha") == 0) {
            var.spi.cpha = to_bool(value);
        } else if (strcmp(key, "spi.order") == 0) {
            var.spi.order = to_order(value);
        } else if (strcmp(key, "spi.baud") == 0) {
            var.spi.baud = to_uint(value);
        } else if (strcmp(key, "spi.autorespond") == 0) {
            if (strcmp(value, "off") == 0) {
                var.spi.has_autorespond = false;
            } else {
                var.spi.has_autorespond = true;
                var.spi.autorespond = to_byte(value);
            }
        } else if (strcmp(key, "spi.output") == 0) {
            var.spi.output = to_output(value);
        } else if (strcmp(key, "i2c.baud") == 0) {
            var.i2c.baud = to_uint(value);
        } else if (strcmp(key, "i2c.output") == 0) {
            var.i2c.output = to_output(value);
        } else {
            syntax_error();
            return;
        }
        variables_save();
    } else {
        syntax_error();
    }
}

const char* from_output(Output output)
{
    switch (output) {
        case ASCII: return "ascii";
        case DEC:   return "dec";
        case BIN:   return "bin";
        case HEX:   return "hex";
    }
}

void variables_print()
{
    printf("uart.baud       = %lu\n", var.uart.baud);
    printf("uart.mode       = %s\n", var.uart.mode);
    printf("uart.hflow      = %s\n", var.uart.hflow ? "on" : "off");
    printf("uart.output     = %s\n", from_output(var.uart.output));
    printf("spi.cpol        = %d\n", var.spi.cpol);
    printf("spi.cpha        = %d\n", var.spi.cpha);
    printf("spi.order       = %s\n", var.spi.order == LSB ? "lsb" : "msb");
    printf("spi.baud        = %lu\n", var.spi.baud);
    if (!var.spi.has_autorespond)
        printf("spi.autorespond = off\n");
    else
        printf("spi.autorespond = 0x%02X\n", var.spi.autorespond);
    printf("spi.output      = %s\n", from_output(var.spi.output));
    printf("i2c.baud        = %lu\n", var.i2c.baud);
    printf("i2c.output      = %s\n", from_output(var.i2c.output));
}
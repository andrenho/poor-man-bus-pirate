#include "variables.h"

#include <string.h>

#include "config.h"

Config config;

void variables_reset()
{
    config.uart.baud = 115200;
    strcpy(config.uart.mode, "81N");
    config.uart.hflow = false;
    config.uart.output = ASCII;

    config.spi.cpol = 0;
    config.spi.cpha = 0;
    config.spi.order = MSB;
    config.spi.baud = 100000;
    config.spi.autorespond = false;
    config.spi.output = HEX;

    config.i2c.baud = 100000;
    config.i2c.output = HEX;
}

void variable_set(const char* key, const char* value)
{
}

void variables_print()
{
}
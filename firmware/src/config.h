#ifndef CONFIG_H_
#define CONFIG_H_

#include <stdint.h>
#include <stdbool.h>

typedef enum { ASCII, DEC, BIN, HEX } Output;

typedef enum { LSB, MSB } Order;

typedef struct {
    uint32_t baud;
    char     mode[4];
    bool     hflow;
    Output   output;
} UARTConfig;

typedef struct {
    bool     cpol;
    bool     cpha;
    Order    order;
    uint32_t baud;
    bool     has_autorespond;
    char     autorespond;
    Output   output;
} SPIConfig;

typedef struct {
    uint32_t baud;
    Output   output;
} I2CConfig;

typedef struct {
    UARTConfig uart;
    SPIConfig  spi;
    I2CConfig  i2c;
} Config;

#endif //CONFIG_H_

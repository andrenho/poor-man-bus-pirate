#ifndef VARIABLES_H_
#define VARIABLES_H_

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
    char     autorespond;
    Output   output;
} SPIConfig;

typedef struct {
    uint32_t baud;
    Output   output;
} I2CConfig;

typedef struct {
    uint32_t freq;
    uint16_t duty;
} PWMConfig;

typedef struct {
    UARTConfig uart;
    SPIConfig  spi;
    I2CConfig  i2c;
    PWMConfig  pwm;
} Config;

extern Config var;

void variables_load();
void variables_reset();
void variables_print();
void variable_set(const char* key, const char* value);

#endif //VARIABLES_H_

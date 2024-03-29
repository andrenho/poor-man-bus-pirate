#include "command.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

#include "uart.h"
#include "variables.h"
#include "spi_slave.h"
#include "spi_master.h"
#include "pwm.h"
#include "i2c_master.h"

#define MAX_TOKENS 6
#define MAX_TOKEN_SZ 20

static void execute(uint8_t n_tokens, char tokens[MAX_TOKENS][MAX_TOKEN_SZ]);

void command_parse(char* line)
{
    char tokens[MAX_TOKENS][MAX_TOKEN_SZ];
    uint8_t n_tokens = 0;

    char* token = strtok(line, " ");
    while (token != NULL) {
        snprintf(tokens[n_tokens++], MAX_TOKEN_SZ - 1, "%s", token);
        token = strtok(NULL, " ");
    }

    execute(n_tokens, tokens);
}

void execute(uint8_t n_tokens, char tokens[MAX_TOKENS][MAX_TOKEN_SZ])
{
    if (n_tokens == 0)
        return;

    if (strcmp(tokens[0], "use") == 0) {
        if (n_tokens == 1) {
            printf("Options: uart, spi_master, spi_slave, spi_sniff, i2c_master, i2c_slave, pmw\n");
        } else if (n_tokens >= 2) {
            if (strcmp(tokens[1], "uart") == 0) {
                if (n_tokens == 3 && strcmp(tokens[2], "guess") == 0)
                    uart_guess_speed();
                else
                    uart_init_();
            } else if (strcmp(tokens[1], "spi_master") == 0) {
                spi_master_init();
            } else if (strcmp(tokens[1], "spi_slave") == 0) {
                spi_slave_init(false);
            } else if (strcmp(tokens[1], "spi_sniff") == 0) {
                spi_slave_init(true);
            } else if (strcmp(tokens[1], "i2c_master") == 0) {
                if (n_tokens == 2) {
                    printf("Options: scan, [slave_address]\n");
                } else if (n_tokens == 3) {
                    if (strcmp(tokens[2], "scan") == 0) {
                        i2c_master_scan();
                    } else {
                        unsigned long addr = strtoul(tokens[2], NULL, 16);
                        if (addr != ULONG_MAX)
                            i2c_master_init(addr);
                        else
                            syntax_error();
                    }
                } else {
                    syntax_error();
                }
            } else if (strcmp(tokens[1], "i2c_slave") == 0) {
                printf("I2C slave is not implemented yet, sorry!\n");
            } else if (strcmp(tokens[1], "pwm") == 0) {
                if (n_tokens == 2)
                    printf("Options: on, off\n");
                else if (n_tokens == 3 && strcmp(tokens[2], "on") == 0)
                    pwm_on();
                else if (n_tokens == 3 && strcmp(tokens[2], "off") == 0)
                    pwm_off();
                else
                    syntax_error();
            } else {
                syntax_error();
            }
        } else {
            syntax_error();
        }
    } else if (strcmp(tokens[0], "reset") == 0 && n_tokens == 1) {
        variables_reset();
    } else if (strcmp(tokens[0], "set") == 0) {
        if (n_tokens == 1)
            variables_print();
        else if (n_tokens == 3)
            variable_set(tokens[1], tokens[2]);
        else
            syntax_error();
    } else {
        syntax_error();
    }
}

void syntax_error()
{
    printf("Syntax error.\n");
}
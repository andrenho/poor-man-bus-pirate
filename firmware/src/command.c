#include "command.h"

#include <stdio.h>
#include <string.h>

#include "uart.h"
#include "variables.h"
#include "spi_slave.h"
#include "spi_master.h"

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
            printf("Options: uart, spi_master, spi_slave, spi_sniff, i2c_master, i2c_salve, pmw0, pmw1\n");
        } else if (n_tokens == 2) {
            if (strcmp(tokens[1], "uart") == 0) {
                uart_init_();
            } else if (strcmp(tokens[1], "spi_master") == 0) {
                spi_master_init();
            } else if (strcmp(tokens[1], "spi_slave") == 0) {
                spi_slave_init(false);
            } else if (strcmp(tokens[1], "spi_sniff") == 0) {
                spi_slave_init(true);
            } else if (strcmp(tokens[1], "i2c_master") == 0) {
            } else if (strcmp(tokens[1], "i2c_slave") == 0) {
            } else if (strcmp(tokens[1], "pwm0") == 0) {
            } else if (strcmp(tokens[1], "pwm1") == 0) {
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
    } else if (strcmp(tokens[0], "guess") == 0) {
        if (n_tokens == 1)
            printf("Options: uart\n");
        else if (n_tokens == 2 && strcmp(tokens[0], "uart") == 0)
            uart_guess_speed();
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
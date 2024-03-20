#include "command.h"

#include <stdio.h>
#include <string.h>

#include "config.h"
#include "variables.h"

#define MAX_TOKENS 6
#define MAX_TOKEN_SZ 20
static const char* SYNTAX_ERROR = "Syntax error.";

static void execute(uint8_t n_tokens, char** tokens);

void command_parse(char* line)
{
    char* tokens[MAX_TOKENS];
    uint8_t token_n = 0;

    char* token = strtok(line, " ");
    while (token != NULL) {
        snprintf(tokens[token_n++], MAX_TOKEN_SZ - 1, "%s", token);
        token = strtok(NULL, " ");
    }

    execute(token_n, tokens);
}

void execute(uint8_t n_tokens, char** tokens)
{
    if (n_tokens == 0)
        return;

    if (strcmp(tokens[0], "hello") == 0 && n_tokens == 1) {
        printf("hello\n");
    } else if (strcmp(tokens[0], "use") == 0 && n_tokens == 2) {
        if (strcmp(tokens[1], "uart") == 0) {
        } else if (strcmp(tokens[1], "spi_master") == 0) {
        } else if (strcmp(tokens[1], "spi_slave") == 0) {
        } else if (strcmp(tokens[1], "spi_sniff") == 0) {
        } else if (strcmp(tokens[1], "i2c_master") == 0) {
        } else if (strcmp(tokens[1], "i2c_slave") == 0) {
        } else if (strcmp(tokens[1], "pwm0") == 0) {
        } else if (strcmp(tokens[1], "pwm1") == 0) {
        } else {
            puts(SYNTAX_ERROR);
        }
    } else if (strcmp(tokens[0], "reset") == 0 && n_tokens == 0) {
        variables_reset();
    } else if (strcmp(tokens[0], "set") == 0) {
        if (n_tokens == 1)
            variables_print();
        else if (n_tokens == 2)
            variable_set(tokens[1], tokens[2]);
        else
            puts(SYNTAX_ERROR);
    } else {
        puts(SYNTAX_ERROR);
    }
}
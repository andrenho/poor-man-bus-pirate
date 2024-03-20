#include "command.h"

#include <stdio.h>
#include <string.h>

#define MAX_TOKENS 6
static const char* SYNTAX_ERROR = "Syntax error.";

static void execute(uint8_t token_n, Token* tokens);

void command_parse(char* line)
{
    Token tokens[MAX_TOKENS];
    uint8_t token_n = 0;

    char* token = strtok(line, " ");
    while (token != NULL) {

        for (Word* word = words; word->str != 0; ++word) {
            if (strcmp(token, word->str) == 0) {
                tokens[token_n++] = word->token;
                if (token_n > MAX_TOKENS)
                    goto done;
            } else {
                puts(SYNTAX_ERROR);
                return;
            }
        }

        token = strtok(NULL, " ");
    }

done:
    execute(token_n, tokens);
}

void execute(uint8_t token_n, Token* tokens)
{
    if (token_n == 0)
        return;

    switch (tokens[0]) {
        case T_HELLO:
            printf("hello\n");
            break;
        default:
            puts(SYNTAX_ERROR);
    }
}
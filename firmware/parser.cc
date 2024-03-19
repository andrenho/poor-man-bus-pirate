#include "parser.hh"
#include "words.hh"

#include <cstdint>
#include <cstring>
#include <cstdio>

#define MAX_TOKENS 6
static const char* SYNTAX_ERROR = "Syntax error.";

static void execute(uint8_t token_n, Token* tokens);

void parse(char* line)
{
    Token tokens[MAX_TOKENS];
    uint8_t token_n = 0;

    char* token = strtok(line, " ");
    while (token != nullptr) {

        for (auto const& word: words) {
            if (strcmp(token, word.str) == 0) {
                tokens[token_n++] = word.token;
                if (token_n > MAX_TOKENS)
                    goto done;
            } else {
                puts(SYNTAX_ERROR);
                return;
            }
        }

        token = strtok(nullptr, " ");
    }

done:
    execute(token_n, tokens);
}

void execute(uint8_t token_n, Token* tokens)
{
    if (token_n == 1) {
        if (tokens[0] == Token::HELLO) {
            printf("hello\n");
        } else {
            puts(SYNTAX_ERROR);
        }
    } else {
        puts(SYNTAX_ERROR);
    }
}
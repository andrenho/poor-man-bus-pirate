#include "buspirate.hh"

#include <cstring>
#include <cstdio>

#define MAX_TOKENS 6
static const char* SYNTAX_ERROR = "Syntax error.";

void BusPirate::parse(char* line)
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

void BusPirate::execute(uint8_t token_n, Token* tokens)
{
    if (token_n == 0)
        return;

    switch (tokens[0]) {
        case Token::HELLO:
            printf("hello\n");
            break;
        default:
            puts(SYNTAX_ERROR);
    }
}
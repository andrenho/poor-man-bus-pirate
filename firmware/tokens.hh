#ifndef TOKENS_HH_
#define TOKENS_HH_

#include <cstdint>

enum class Token {
    HELLO
};

static struct Word {
    const char* str;
    Token       token;
} words[] = {
    { "hello", Token::HELLO },
};

#endif //TOKENS_HH_

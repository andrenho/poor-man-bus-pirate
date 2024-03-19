#ifndef WORDS_HH_
#define WORDS_HH_

#include <cstdint>

enum class Token {
    HELLO,
    EOF
};

struct Word {
    const char* str;
    Token       token;
} words[] = {
    { "hello", Token::HELLO },
};

#endif //WORDS_HH_

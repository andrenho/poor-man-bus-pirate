#ifndef TOKENS_H_
#define TOKENS_H_

#include <stdint.h>

typedef enum {
    T_HELLO,
    T_NULL
} Token;

typedef struct Word {
    const char* str;
    Token       token;
} Word;

static Word words[] = {
    { "hello", T_HELLO },
    { 0, T_NULL },
};

#endif //TOKENS_H_

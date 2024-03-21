#include "output.h"

#include <stdio.h>

static Output output_mode = ASCII;
static Color  last_color = C_NONE;

void output_set_mode(Output output)
{
    output_mode = output;
}

static void output_set_color(Color color)
{
    if (color != last_color) {
        switch (color) {
            case C_INPUT:
                printf("\e[1;32m");
                break;
            case C_OUTPUT:
                printf("\e[1;36m");
                break;
            case C_SNIFF:
                printf("\e[1;33m");
                break;
            case C_NONE:
                printf("\e[0m");
                break;
        }
        last_color = color;
    }
}

void output_print(const char* s, Color color)
{
    output_set_color(color);
    printf("%s", s);
}

#define CHAR_BIT 8
static int printb(uintmax_t n)
{
    char b[(sizeof n * CHAR_BIT) + 1];
    char *p = b + sizeof b;
    *--p = '\0';
    do {
        *--p = '0' + (n & 1);
    } while (n >>= 1);
    return printf("%s", p);
}


void output_putchar(char c, Color color)
{
    output_set_color(color);
    switch (output_mode) {
        case ASCII:
            if (c >= 32 && c < 127)
                putchar(c);
            else
                printf("[%02X]", c);
            break;
        case DEC:
            printf("%d ", c);
            break;
        case HEX:
            printf("%02X ", c);
            break;
        case BIN:
            printb(c);
            break;
    }
}
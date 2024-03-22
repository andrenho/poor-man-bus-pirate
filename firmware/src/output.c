#include "output.h"

#include <stdio.h>

#include "pico/util/queue.h"

#define QUEUE_COUNT 3
#define QUEUE_SIZE  512

static Output output_mode = ASCII;
static Channel  last_color = C_NONE;

queue_t queue[QUEUE_COUNT];

void output_init()
{
    for (size_t i = 0; i < QUEUE_COUNT; ++i)
        queue_init(&queue[i], 1, QUEUE_SIZE);
}

void output_set_mode(Output output)
{
    output_mode = output;
}

static void output_set_color(Channel color)
{
    if (color != last_color) {
        switch (color) {
            case C_INPUT:
                printf("\e[1;32m");
                break;
            case C_OUTPUT:
                printf("\e[1;31m");
                break;
            case C_SNIFF:
                printf("\e[1;34m");
                break;
            case C_NONE:
                printf("\e[0m");
                break;
        }
        last_color = color;
    }
}

void output_print(const char* s, Channel channel)
{
    output_set_color(channel);
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

static void output_putchar(char c, Channel channel)
{
    output_set_color(channel);
    switch (output_mode) {
        case ASCII:
            if (c == 0x1b)
                printf("[ESC]");
            else
                putchar(c);
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

void output_queue_add(char c, Channel channel)
{
    if (!queue_try_add(&queue[channel], &c)) {
        // TODO - do something if the queue is full
    }
}

void output_print_queues()
{
    char c;
    for (size_t i = 0; i < QUEUE_COUNT; ++i)
        while (queue_try_remove(&queue[i], &c))
            output_putchar(c, i);
}
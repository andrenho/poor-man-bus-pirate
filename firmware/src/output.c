#include "output.h"

#include <stdio.h>
#include <pico/stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <limits.h>

#include "pico/util/queue.h"

#define QUEUE_SIZE  512
#define INPUT_MAX   9

static Output  output_mode = ASCII;
static Channel last_color = C_NONE;
static char    input[INPUT_MAX] = {0};

typedef struct {
    char    c       : 8;
    Channel channel : 8;
} QueueItem;

static queue_t queue;

void output_init()
{
    queue_init(&queue, sizeof(QueueItem), QUEUE_SIZE);
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

static int printb(uintmax_t n)
{
    char b[(sizeof n * 8) + 1];
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
    QueueItem item = { .c = c, .channel = channel };
    if (!queue_try_add(&queue, &item)) {
        // TODO - do something if the queue is full
    }
}

void output_print_queues()
{
    QueueItem item;
    while (queue_try_remove(&queue, &item))
        output_putchar(item.c, item.channel);
}

int output_get_char()
{
    int c = getchar_timeout_us(0);
    if (c == 0x3)
        return CTRL_C;
    else if (c == PICO_ERROR_TIMEOUT)
        return NO_CHAR;

    if (output_mode == ASCII) {
        return c;
    } else {
        unsigned int len = strlen(input);
        if (isxdigit(c) && len < INPUT_MAX) {
            input[len] = c;
            input[len+1] = '\0';
            return NO_CHAR;
        } else if (c != '\r' && c != '\n') {
            return NO_CHAR;
        } else {  // enter
            unsigned long v = strtoul(input, NULL, output_mode == DEC ? 10 : (output_mode == HEX ? 16 : 2));
            output_set_color(C_NONE);
            if (v == ULONG_MAX || v > 0xff) {
                printf("[ERR]");
                input[0] = '\0';
                return NO_CHAR;
            } else if (output_mode == HEX) {
                printf("[%02X]", (unsigned int) v);
            } else if (output_mode == DEC) {
                printf("[%d]", (unsigned int) v);
            } else if (output_mode == BIN) {
                printb(v);
            }
            input[0] = '\0';
            return ((int) v) & 0xff;
        }
    }
}
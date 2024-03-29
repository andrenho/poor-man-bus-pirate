#ifndef OUTPUT_H_
#define OUTPUT_H_

#include "variables.h"

typedef enum { C_INPUT = 0, C_OUTPUT = 1, C_SNIFF = 2, C_NONE } Channel;

void output_init();
void output_set_mode(Output output);
void output_print(const char* s, Channel channel);

void output_queue_add(char c, Channel channel);
void output_print_queues();

#define NO_CHAR ((int) -1)
#define CTRL_C ((int) -2)
#define CTRL_E ((int) -3)
#define CTRL_F ((int) -4)
#define CTRL_G ((int) -5)
#define CTRL_H ((int) -6)
#define CTRL_I ((int) -7)
#define CTRL_K ((int) -8)
int output_get_char();

#endif //OUTPUT_H_

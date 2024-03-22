#ifndef OUTPUT_H_
#define OUTPUT_H_

#include "variables.h"

typedef enum { C_INPUT = 0, C_OUTPUT = 1, C_SNIFF = 2, C_NONE } Channel;

void output_init();
void output_set_mode(Output output);
void output_print(const char* s, Channel channel);

void output_queue_add(char c, Channel channel);
void output_print_queues();

#define CTRL_C ((int) -1)
#define NO_CHAR ((int) -2)
int output_get_char();

#endif //OUTPUT_H_

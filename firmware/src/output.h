#ifndef OUTPUT_H_
#define OUTPUT_H_

#include "variables.h"

typedef enum { C_INPUT, C_OUTPUT, C_SNIFF, C_NONE } Color;

void output_set_mode(Output output);
void output_print(const char* s, Color color);
void output_putchar(char c, Color color);

#endif //OUTPUT_H_

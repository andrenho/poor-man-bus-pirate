#include "pico/stdlib.h"

#include <stdio.h>

#include "util/getline.h"
#include "command.h"
#include "variables.h"

int main()
{
	stdio_init_all();

    printf("Welcome to Poor Man Bus Pirate!\n");
    printf("Available commands: set, reset, use\n");

    variables_load();

    char buf[256];
	while (true) {
        printf("> ");
        getline(buf, sizeof buf);
        command_parse(buf);
	}
}
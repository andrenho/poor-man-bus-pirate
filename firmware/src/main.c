#include "pico/stdlib.h"

#include <stdio.h>

#include "util/getline.h"
#include "command.h"
#include "variables.h"
#include "output.h"

int main()
{
	stdio_usb_init();

    printf("Welcome to Poor Man Bus Pirate!\n");
    printf("Available commands: set, reset, use, guess\n");

    output_init();
    variables_load();

    char buf[256];
	while (true) {
        printf("> ");
        getline(buf, sizeof buf);
        command_parse(buf);
	}
}
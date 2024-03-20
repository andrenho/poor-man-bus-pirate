#include "pico/stdlib.h"

#include <stdio.h>

#include "util/getline.h"
#include "command.h"

int main()
{
	stdio_init_all();

    char buf[256];
	while (true) {
        printf("> ");
        getline(buf, sizeof buf);
        command_parse(buf);
	}
}

// https://pico.pinout.xyz/
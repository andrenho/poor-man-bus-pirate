#include "pico/stdlib.h"

#include <cstdio>
#include <cstdlib>

#include "util/getline.hh"

int main()
{
	stdio_init_all();

    char buf[512];
	while (true) {
        printf("> ");
        char* buf = getLine(true, '\r');
        puts("");
        // call REPL
        free(buf);
	}
}

#include "pico/stdlib.h"

#include <cstdio>
#include <cstdlib>

#include "util/getline.hh"
#include "parser.hh"

int main()
{
	stdio_init_all();

    char buf[256];
	while (true) {
        printf("> ");
        getline(buf, sizeof buf);
        parse(buf);
	}
}

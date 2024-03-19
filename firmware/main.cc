#include "pico/stdlib.h"

#include <cstdio>

#include "util/getline.hh"
#include "buspirate.hh"

int main()
{
	stdio_init_all();

    BusPirate bus_pirate;

    char buf[256];
	while (true) {
        printf("> ");
        getline(buf, sizeof buf);
        bus_pirate.parse(buf);
	}
}

#include "pico/stdlib.h"

#include <stdio.h>

#include "util/getline.h"
#include "command.h"
#include "variables.h"
#include "output.h"
#include "pins.h"

#define LED_PIN 25

int main()
{
	stdio_usb_init();
    pins_reset();

    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    gpio_put(LED_PIN, 1);

    getchar();

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
#include "pins.h"

#include <hardware/gpio.h>

void pins_reset()
{
    for (size_t i = 0; i < 15; ++i) {
        gpio_set_function(i, GPIO_FUNC_NULL);
        gpio_set_dir(i, false);
        gpio_set_pulls(i, true, false);
    }
}
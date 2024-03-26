#include "pwm.h"

#include <hardware/clocks.h>
#include <hardware/gpio.h>
#include <hardware/pwm.h>

#include "variables.h"

bool pwm_is_on = false;

void pwm_on()
{
    gpio_set_function(0, GPIO_FUNC_PWM); // Tell GPIO 0 it is allocated to the PWM
    uint slice_num = pwm_gpio_to_slice_num(0); // get PWM slice for GPIO 0 (it's slice 0)

    // set frequency
    // determine top given Hz - assumes free-running counter rather than phase-correct
    uint32_t f_sys = clock_get_hz(clk_sys); // typically 125'000'000 Hz
    float divider = f_sys / 125000000UL;  // let's arbitrarily choose to run pwm clock at 1MHz
    pwm_set_clkdiv(slice_num, divider); // pwm clock should now be running at 1MHz
    uint32_t top = 125000000UL / var.pwm.freq -1; // TOP is u16 has a max of 65535, being 65536 cycles
    pwm_set_wrap(slice_num, top);

    // set duty cycle
    uint16_t level = (top+1) * var.pwm.duty / 100 -1; // calculate channel level from given duty cycle in %
    pwm_set_chan_level(slice_num, 0, level);

    pwm_set_enabled(slice_num, true); // let's go!

    pwm_is_on = true;
}

void pwm_off()
{
    uint slice_num = pwm_gpio_to_slice_num(0); // get PWM slice for GPIO 0 (it's slice 0)
    pwm_set_enabled(slice_num, false);
    gpio_set_input_enabled(0, true);

    pwm_is_on = false;
}

// https://forums.raspberrypi.com/viewtopic.php?t=303116

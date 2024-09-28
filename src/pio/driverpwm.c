#include <stdio.h>

#include <pico/stdlib.h>
#include <hardware/pio.h>
#include "driverpwm.pio.h"

#include "driverpwm.h"

void pio_driverpwm_set_config(PIO pio, uint sm, uint32_t period)
{
    pio_sm_set_enabled(pio, sm, false);
    pio_sm_put_blocking(pio, sm, period);
    pio_sm_exec(pio, sm, pio_encode_pull(false, false));
    pio_sm_exec(pio, sm, pio_encode_out(pio_isr, 32));
    pio_sm_set_enabled(pio, sm, true);
}

void pio_driverpwm_set_level(PIO pio, uint sm, uint32_t level, bool isPinTwo)
{
    pio_sm_put_blocking(pio, sm, (level << 1) | (isPinTwo ? 1 : 0));
}
#include <stdio.h>

#include <pico/stdlib.h>
#include <hardware/pio.h>
#include "driverpwm.pio.h"

#ifdef __cplusplus
extern "C"
{
#endif
    void pio_driverpwm_set_config(PIO pio, uint sm, uint32_t period);

    /// @brief Set PWM speed of state machine
    /// @param pio The target PIO
    /// @param sm The target state machine
    /// @param level A 31-bit magnitude to set the PWM to (the actual range is set by the pwm period)
    /// @param isPinTwo When true, pulses the second pin instead of the first
    void pio_driverpwm_set_level(PIO pio, uint sm, uint32_t level, bool isPinTwo);
#ifdef __cplusplus
}
#endif
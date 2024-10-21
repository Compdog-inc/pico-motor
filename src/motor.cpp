#include <pico/stdlib.h>
#include <limits.h>
#include <math.h>
#include <hardware/pio.h>
#include "pio/driverpwm.h"
#include "motor.h"
#include "config.h"

// pio0 program address
static uint pio0Offset = UINT_MAX;
// pio1 program address
static uint pio1Offset = UINT_MAX;
// next available state machine index
static uint8_t smIndex = 0;

// valid state machines to use
static const uint8_t sm_values[] = {
    USED_STATE_MACHINES};

constexpr uint32_t MOTOR_PWM_PERIOD = (1u << 16) - 1; // 16-bit pwm resolution

Motor::Motor(uint cw, uint ccw) : currentSpeed(0)
{
    // pio program uses a base+offset pin assignment
    if (cw < ccw)
    {
        assert(ccw == cw + 1);
        base = cw;
        baseIsCcw = false;
    }
    else
    {
        assert(cw == ccw + 1);
        base = ccw;
        baseIsCcw = true;
    }

    assert(smIndex < sizeof(sm_values));

    sm = sm_values[smIndex++];
    pio = sm < 4 ? pio0 : pio1; // pio0 is sm 0 - 3, pio1 is 4 - 6
    sm %= 4;                    // sm index relative to pio

    // if creating a new Motor for the first time, load the pio program
    if (pio == pio0 && pio0Offset == UINT_MAX)
    {
        pio0Offset = pio_add_program(pio, &driverpwm_program);
    }
    else if (pio == pio1 && pio1Offset == UINT_MAX)
    {
        pio1Offset = pio_add_program(pio, &driverpwm_program);
    }

    // init pio program with base pin
    driverpwm_program_init(pio, sm, pio == pio0 ? pio0Offset : pio1Offset, base);
    pio_driverpwm_set_config(pio, sm, MOTOR_PWM_PERIOD);
    pio_sm_set_clkdiv(pio, sm, 20.f);
    set(0);
}

Motor::~Motor()
{
    gpio_deinit(base);
    gpio_deinit(base + 1);
}

void Motor::set(float speed)
{
    assert(speed >= -1);
    assert(speed <= 1);

    currentSpeed = speed;
    speed *= (speed > 0 ? 1 : -1) * speed;                        // Squared PWM response curve, with sign
    uint16_t level = (uint16_t)floorf(fabsf(speed) * UINT16_MAX); // pio program uses 16-bit speed values
    if (speed >= 0)
    {
        pio_driverpwm_set_level(pio, sm, level, baseIsCcw);
    }
    else
    {
        pio_driverpwm_set_level(pio, sm, level, !baseIsCcw);
    }
}

float Motor::getSetSpeed()
{
    return currentSpeed;
}
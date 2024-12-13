#include <pico/stdlib.h>
#include <limits.h>
#include <math.h>
#include <hardware/pio.h>
#include <hardware/clocks.h>
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

inline uint32_t get_cycles_from_resolution(uint32_t resolution)
{
    return 3 * resolution + 10;
}

inline float get_clkdiv_from_hz(float hz, uint32_t resolution)
{
    uint32_t clk = clock_get_hz(clk_sys);
    return (float)clk / (get_cycles_from_resolution(resolution) * hz);
}

inline float get_hz_from_clkdiv(float clkdiv, uint32_t resolution)
{
    // inverse of itself
    return get_clkdiv_from_hz(clkdiv, resolution);
}

float Motor::maxAllowedFrequency() const
{
    return get_hz_from_clkdiv(1, resolution); // clkdiv can't go lower than 1
}

float Motor::maxAllowedFrequency(uint32_t resolution)
{
    return get_hz_from_clkdiv(1, resolution); // clkdiv can't go lower than 1
}

Motor::Motor(uint cw, uint ccw, float frequency, uint32_t resolution) : currentSpeed(0), frequency(frequency), resolution(resolution)
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

    assert(resolution >= 0);
    assert(resolution <= MAX_MOTOR_PWM_RESOLUTION);
    assert(frequency > 0);
    assert(frequency <= maxAllowedFrequency());

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
    pio_driverpwm_set_config(pio, sm, resolution);
    pio_sm_set_clkdiv(pio, sm, get_clkdiv_from_hz(frequency, resolution));
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

    uint16_t level = (uint16_t)floorf(fabsf(speed) * resolution); // pio program uses raw counter values, scale by speed
    if (speed >= 0)
    {
        pio_driverpwm_set_level(pio, sm, level, baseIsCcw);
    }
    else
    {
        pio_driverpwm_set_level(pio, sm, level, !baseIsCcw);
    }
}

float Motor::getSetSpeed() const
{
    return currentSpeed;
}

void Motor::setFrequency(float frequency)
{
    assert(frequency > 0);
    assert(frequency <= maxAllowedFrequency());
    this->frequency = frequency;
    pio_sm_set_clkdiv(pio, sm, get_clkdiv_from_hz(frequency, resolution));
}

float Motor::getFrequency() const
{
    return frequency;
}

void Motor::setResolution(uint32_t resolution)
{
    assert(resolution >= 0);
    assert(resolution <= MAX_MOTOR_PWM_RESOLUTION);
    this->resolution = resolution;
    pio_driverpwm_set_config(pio, sm, resolution);
    pio_sm_set_clkdiv(pio, sm, get_clkdiv_from_hz(frequency, resolution));
    set(currentSpeed); // update set_level with new resolution
}

uint32_t Motor::getResolution() const
{
    return resolution;
}
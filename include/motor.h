#ifndef _MOTOR_H_
#define _MOTOR_H_

#include <stdlib.h>
#include <hardware/pio.h>

/// @brief A PWM DC motor controller using PIO
class Motor
{
public:
    /// @brief Create a new motor
    /// @param cw The clockwise GPIO pin
    /// @param ccw The counter-clockwise GPIO pin
    /// @warning The two pins have to be next to each other!
    Motor(uint cw, uint ccw);
    ~Motor();

    /// @brief Set the motor percent speed
    /// @param speed The speed (-1 to 1)
    void set(float speed);

    /// @brief Returns the currently set percent speed (-1 to 1)
    float getSetSpeed();

private:
    PIO pio;
    uint sm;
    uint base;
    bool baseIsCcw;
    float currentSpeed;
};

#endif
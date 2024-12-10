#ifndef _MOTOR_H_
#define _MOTOR_H_

#include <stdlib.h>
#include <hardware/pio.h>

constexpr uint32_t MAX_MOTOR_PWM_RESOLUTION = (1u << 16) - 1;     // 16-bit pwm resolution is the highest supported resolution (up to around 600 Hz)
constexpr uint32_t DEFAULT_MOTOR_PWM_RESOLUTION = (1u << 11) - 1; // 11-bit pwm resolution (up to around 20 kHz - best for motors)

/// @brief A PWM DC motor controller using PIO
class Motor
{
public:
    /// @brief Create a new motor
    /// @param cw The clockwise GPIO pin
    /// @param ccw The counter-clockwise GPIO pin
    /// @param frequency The frequency of the PWM in Hz - default is 20 kHz
    /// @param resolution The resolution (max counter value) of the PWM - default is 11-bit, just enough for 20 kHz
    /// @note The lower the resolution, the higher the maximum reachable frequency is, but with less speed steps
    /// @warning The two pins have to be next to each other!
    Motor(uint cw, uint ccw, float frequency = 20000.0f /* 20 kHz */, uint32_t resolution = DEFAULT_MOTOR_PWM_RESOLUTION);
    ~Motor();

    /// @brief Set the motor percent speed
    /// @param speed The speed (-1 to 1)
    void set(float speed);
    /// @brief Returns the currently set percent speed (-1 to 1)
    float getSetSpeed() const;

    /// @brief Sets the frequency of the motor PWM in Hz
    /// @param frequency The frequency in Hz
    void setFrequency(float frequency);
    /// @brief Returns the current frequency of the motor PWM in Hz
    float getFrequency() const;

    void setResolution(uint32_t resolution);
    uint32_t getResolution() const;

    /// @brief Returns the maximum reachable frequency in Hz with the current PWM resolution
    float maxAllowedFrequency() const;

    /// @brief Returns the maximum reachable frequency in Hz with the provided PWM resolution
    static float maxAllowedFrequency(uint32_t resolution);

private:
    PIO pio;
    uint sm;
    uint base;
    bool baseIsCcw;
    float currentSpeed;
    float frequency;
    uint32_t resolution;
};

#endif
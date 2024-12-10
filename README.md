# pico-motor
A DC motor controller library for the Raspberry PI Pico

### Features:

- 16-bit DC motor PWM
- Customizable PWM resolution up to 16-bit
- Support for up to 7 independent motors
- Uses PIO state machines
- Direction is determined H-bridge style, one pin is low and the other is pulsed.
- Customizable frequency (within resolution limit `Motor::maxAllowedFrequency()`)
- Default 11-bit PWM resolution and 20 kHz frequency for optimal motor control

### Config Options (CMake)

- `PICO_MOTOR_USED_STATE_MACHINES` (default: `0,1,2,3,5,6` - 4 is skipped). Lists the state machines to use for motor PWM.

PWM frequency and period diagram (16-bit / 31.78 Hz):  

<img width="755" alt="PWM frequency and period diagram" src="https://github.com/user-attachments/assets/b908711b-0ce4-4950-a07d-a95f3d5e49b7">

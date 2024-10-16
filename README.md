# pico-motor
A DC motor controller library for the Raspberry PI Pico

### Features:

- 16-bit DC motor PWM
- Support for up to 7 independent motors
- Uses PIO state machines
- Direction is determined H-bridge style, one pin is low and the other is pulsed.

### Config Options (CMake)

- `PICO_MOTOR_USED_STATE_MACHINES` (default: `0,1,2,3,5,6` - 4 is skipped). Lists the state machines to use for motor PWM.

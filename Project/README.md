
# PWM Control for AVR Microcontroller

This project demonstrates how to use timers and interrupts on an AVR microcontroller to control Pulse Width Modulation (PWM) signals on multiple pins. The program configures three timers to generate PWM signals on different pins and changes their duty cycles in response to timer overflows.

## Features

- **Multiple PWM Channels**: Controls PWM on 5 pins using three timers (Timer0, Timer1, and Timer2).
- **Duty Cycle Modification**: Increases the duty cycle by 10% on each interrupt, wrapping around back to 0% after 100%.
- **Timer Interrupts**: Uses overflow interrupts from each timer to trigger duty cycle updates.
- **Efficient Timer Usage**: Utilizes both 8-bit and 16-bit timers for PWM generation.

## Hardware Requirements

- **AVR Microcontroller**: Any AVR microcontroller with support for timers (e.g., ATmega328P, ATmega16).
- **LEDs or Other PWM Devices**: Connect PWM-controlled devices to the output pins (PD6, PD5, PB1, PB2, PB3).

## Pin Configuration

- **PD6**: Connected to Timer0 (PWM output)
- **PD5**: Connected to Timer0 (PWM output)
- **PB1**: Connected to Timer1 (PWM output)
- **PB2**: Connected to Timer1 (PWM output)
- **PB3**: Connected to Timer2 (PWM output)

## Code Explanation

- **PWM_Init()**: Configures Timer0, Timer1, and Timer2 for Fast PWM mode and sets prescalers.
- **update_PWM()**: Updates the duty cycles for each PWM signal based on the `dutycycle` array.
- **Interrupts**: Each timer overflow triggers an interrupt that increases the duty cycle of all PWM signals by 10%.

## Compilation and Usage

To compile and upload this code to your AVR microcontroller, use the following tools:

- **AVR-GCC** for compilation.
- **AVRDUDE** for programming the microcontroller.

Run the following commands:

```bash
avr-gcc -mmcu=atmega328p -o pwm_control.elf pwm_control.c
avr-objcopy -O ihex pwm_control.elf pwm_control.hex
avrdude -c usbasp -p m328p -U flash:w:pwm_control.hex:i

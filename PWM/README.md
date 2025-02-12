# AVR PWM Control using Timer0

## Overview
This project generates a PWM signal on the PD6 pin of an AVR microcontroller using Timer0 in fast PWM mode. The duty cycle of the PWM signal is periodically updated using an overflow interrupt.

## Features
- Configures Timer0 in fast PWM mode
- Outputs PWM signal on PD6
- Uses Timer0 overflow interrupt to update duty cycle
- Simple implementation with incremental duty cycle updates

## Hardware Requirements
- AVR Microcontroller (e.g., ATmega328P)
- LED (optional for visual indication)
- 16MHz Clock Frequency

## Software Requirements
- AVR-GCC
- AVRDUDE (for programming the microcontroller)
- Makefile or command-line tools for compilation

## How It Works
1. The program initializes Timer0 in fast PWM mode.
2. The initial duty cycle is set to 25%.
3. The overflow interrupt increments the duty cycle periodically.
4. Once the duty cycle exceeds 100%, it resets to 0%.
5. The new duty cycle is applied to OCR0A, modifying the PWM output.

## Usage
- Compile the code using `avr-gcc`:
  ```sh
  avr-gcc -mmcu=atmega328p -o pwm.elf main.c

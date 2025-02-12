# AVR ADC to PWM Control

## Description

This project reads an analog voltage using the ADC of an AVR microcontroller and generates a PWM signal based on the input voltage level. The PWM duty cycle is adjusted dynamically depending on the ADC readings.

## Features

- Uses **ADC0** channel for reading analog input
- Converts ADC values into voltage
- Generates **PWM** signal using Timer0
- Adjusts **PWM duty cycle** dynamically based on voltage levels
- Uses **Timer0 overflow interrupt**

## Hardware Requirements

- **AVR microcontroller** (e.g., ATmega328P)
- **5V Analog input** source
- **PWM output pin** connected to an LED or other output device

## Software Requirements

- **AVR-GCC**
- **AVRDUDE**
- **Makefile**
- **AVR Programmer**

## Code Structure

- `main.c` – Contains the main logic of ADC reading and PWM control.
- `ADC_init()` – Initializes ADC.
- `PWM_init()` – Configures Timer0 for PWM generation.
- `Read_ADC()` – Reads the ADC value.
- `Convert_Voltage()` – Converts ADC value to voltage.
- `generate_PWM()` – Generates PWM duty cycle based on voltage.

## Usage

1. Compile the code using AVR-GCC.
2. Upload the firmware to the AVR microcontroller.
3. Provide an analog voltage to the **ADC0** pin.
4. Observe PWM output on **PD6**.

## License

This project is open-source and available for modification and redistribution.

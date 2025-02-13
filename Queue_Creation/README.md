# FreeRTOS Queue Example on STM32F446RE

## Overview
This project demonstrates the use of FreeRTOS queues on an STM32F446RE microcontroller. It consists of two tasks:
1. **Sender Task** - Sends integer values to a queue periodically.
2. **Receiver Task** - Receives values from the queue and toggles an LED.

The project uses the USART2 peripheral for UART communication and GPIO for LED control.

## Hardware Requirements
- STM32F446RE microcontroller
- LED connected to GPIOA Pin 5
- UART (USART2) for debugging output

## Software Requirements
- STM32CubeIDE
- FreeRTOS
- HAL (Hardware Abstraction Layer)

## Queue Mechanism
- A queue of **length 5** is created using:
  ```c
  xQueue = xQueueCreate(5, sizeof(int));
  ```
- The **Sender Task** sends an incrementing integer to the queue every 500ms.
- The **Receiver Task** retrieves the integer and toggles an LED on GPIOA Pin 5.
- If the queue is full, the sender waits for 1000ms before failing.

## Code Structure

### **Main Function (`main.c`)**
- Initializes peripherals (GPIO, UART, Clock)
- Creates the FreeRTOS queue
- Creates two tasks (`vSenderTask` and `vReceiverTask`)
- Starts the FreeRTOS scheduler

### **Sender Task (`vSenderTask`)**
- Attempts to send an integer to the queue.
- If successful, prints the sent value via UART.
- Delays for 500ms before sending the next value.

### **Receiver Task (`vReceiverTask`)**
- Waits for data from the queue (blocks for 1000ms).
- If data is received, prints the value via UART and toggles the LED.

## Expected UART Output
```
Queue value sent successfully
Sent Value: 0
Received Value: 0
Queue value sent successfully
Sent Value: 1
Received Value: 1
```

## Building and Running
1. Open the project in STM32CubeIDE.
2. Build the project.
3. Flash it onto the STM32 board.
4. Observe the LED blinking and UART output.

## License
This project is licensed under STMicroelectronics standard terms (AS-IS).


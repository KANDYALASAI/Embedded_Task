# FreeRTOS-Based STM32 LED Blinking Project

## Overview
This project demonstrates the use of FreeRTOS on an STM32 microcontroller to achieve multitasking. Two tasks are created to blink LEDs at different intervals using FreeRTOS task scheduling.

## Features
- Implements FreeRTOS for multitasking.
- Two independent tasks control separate LEDs.
- Uses `vTaskDelay()` for task scheduling.
- Demonstrates preemptive scheduling based on task priority.

## Hardware Requirements
- **STM32F4** microcontroller (tested on STM32F446RE)
- **GPIO Pins**:
  - PA5 (LED 1)
  - PA6 (LED 2)

## Software Requirements
- **STM32CubeIDE** (for development and debugging)
- **FreeRTOS** (included in the project)

## Project Structure
```
project_folder/
│-- Core/
│   ├── Inc/
│   │   ├── main.h
│   ├── Src/
│   │   ├── main.c
│   │   ├── freertos.c
│-- Drivers/
│-- FreeRTOS/
│-- README.md
```

## Task Details
| Task Name     | Function             | GPIO Pin | Frequency |
|--------------|---------------------|----------|-----------|
| GreenLedTask1 | Toggles LED 1 (PA5) | PA5      | 1000 ms   |
| GreenLedTask2 | Toggles LED 2 (PA6) | PA6      | 2000 ms   |

## Code Explanation
### Task Creation (main.c)
```c
xTaskCreate(vGreenLedTask1, "Green Led Task1", 128, NULL, 1, &xGreenLedTask1Handle);
xTaskCreate(vGreenLedTask2, "Green Led Task2", 128, NULL, 2, &xGreenLedTask2Handle);
vTaskStartScheduler();
```
- **vGreenLedTask1**: Priority 1
- **vGreenLedTask2**: Priority 2 (higher priority than Task1)

### Task Function (LED Blinking)
```c
void vGreenLedTask1(void *pvParameters)
{
    for(;;)
    {
        HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
```
```c
void vGreenLedTask2(void *pvParameters)
{
    for(;;)
    {
        HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_6);
        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}
```
- Each task runs in an infinite loop, toggles the respective LED, and then sleeps for a specific delay.
- `vTaskDelay()` suspends the task for the given duration, allowing other tasks to execute.

## How Multitasking Works
- **Task2 (priority 2)** runs first, toggles LED 2, and sleeps for 2000ms.
- **Task1 (priority 1)** runs next, toggles LED 1, and sleeps for 1000ms.
- The scheduler ensures Task1 runs again after 1000ms, while Task2 resumes after 2000ms.

## Building and Flashing
1. Open the project in **STM32CubeIDE**.
2. Build the project (`Ctrl + B`).
3. Connect your STM32 board.
4. Flash the firmware (`Run -> Debug` or `Ctrl + F11`).

## Expected Output
- LED1 (PA5) blinks every 1 second.
- LED2 (PA6) blinks every 2 seconds.

## License
This project is open-source and provided AS-IS without warranty.

---
**Author:** kandyala sai kumar 
**Date:** january 2025


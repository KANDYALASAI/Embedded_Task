/**
 * @file main.c
 * @brief PWM control using Timer0 on AVR
 *
 * This program generates a PWM signal on PD6 using Timer0 in fast PWM mode.
 * The duty cycle is updated periodically using an overflow interrupt.
 *
 * @author kANDYALA SAI KUMAR
 * @date 2025-01-07
 */

#define F_CPU 16000000UL  /**< CPU Clock Frequency */
#include <avr/io.h>
#include <util/delay.h>
#include <stdbool.h>
#include <avr/interrupt.h>

/** @brief Constants for PWM calculation */
#define DIV_FACTOR 100.0  /**< Division factor for duty cycle calculation */
#define MAX_VALUE 255.0   /**< Maximum PWM register value */
#define DELAY_MS 10000    /**< Delay in milliseconds */
#define DCCINCFCTR 10     /**< Duty cycle increment factor */
#define MAXDC 100         /**< Maximum duty cycle percentage */
#define MINDC 0           /**< Minimum duty cycle percentage */

int dutycycle = 25;            /**< Initial duty cycle value */
volatile bool intrpt_rcvd = false; /**< Interrupt received flag */

/**
 * @brief Main function to configure and start PWM
 *
 * Configures Timer0 in fast PWM mode and sets up an overflow interrupt.
 * The duty cycle is updated inside the interrupt handler.
 *
 * @return int Return 0 on successful execution.
 */
int main(void)
{
    // Set PD6 as an output for PWM
    DDRD = (1 << PORTD6);

    // Configure Timer0 in Fast PWM mode, non-inverting mode
    TCCR0A = (1 << WGM00) | (1 << WGM01) | (1 << COM0A1);

    // Enable Timer0 overflow interrupt
    TIMSK0 = (1 << TOIE0);

    // Set initial duty cycle value to OCR0A
    OCR0A = ((dutycycle / DIV_FACTOR) * MAX_VALUE);

    // Enable global interrupts
    sei();

    // Start the timer with no prescaler
    TCCR0B = (1 << CS00);

    while (1)
    {
        if (intrpt_rcvd == true)
        {
            _delay_ms(1000);

            // Update duty cycle
            dutycycle += DCCINCFCTR;
            if (dutycycle > MAXDC)
            {
                dutycycle = MINDC;
            }
            intrpt_rcvd = false; // Reset flag

            // Set new duty cycle value to OCR0A
            OCR0A = ((dutycycle / DIV_FACTOR) * MAX_VALUE);
        }
    }

    return 0;
}

/**
 * @brief Timer0 Overflow Interrupt Service Routine
 *
 * This ISR sets the interrupt received flag to true, signaling the main loop
 * to update the PWM duty cycle.
 */
ISR(TIMER0_OVF_vect)
{
    intrpt_rcvd = true;
}

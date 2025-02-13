/**
 * @file led_brigtness.c
 * @brief led_brightness control for multiple pins using timers and interrupts.
 *
 * This file contains functions to initialize and control PWM signals for
 * various pins using the AVR microcontroller. It utilizes the 8-bit and 16-bit
 * timers to generate PWM waveforms, and uses interrupts to modify the duty cycle
 * of the PWM signals.
 *
 * @author Kandyala sai kumar
 * @date 2025-01-13
 * @version 1.0
 */

#define F_CPU 16000000UL /**< Clock frequency definition for delay functions. */
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdbool.h>

/**
 * @def MAXDC
 * @brief Maximum duty cycle value (100%).
 */
#define MAXDC 100

/**
 * @def MINDC
 * @brief Minimum duty cycle value (0%).
 */
#define MINDC 0

/**
 * @def MAX_VALUE
 * @brief Maximum timer value (255).
 */
#define MAX_VALUE 255

/**
 * @brief Array to store the duty cycle values for 5 PWM outputs.
 *
 * This array holds the duty cycle for each PWM channel. The values are
 * expressed as percentages of the maximum duty cycle.
 */
volatile uint8_t dutycycle[5] = {10, 20, 30, 40, 50};

/**
 * @brief Interrupt flags for each timer overflow interrupt.
 *
 * This array is used to track if an interrupt has been received for a particular
 * timer overflow.
 */
volatile bool intrpt_rcvd[3] = {false, false, false};

/**
 * @brief Initializes PWM signals for multiple pins using timers and interrupts.
 *
 * This function configures the necessary timers (Timer0, Timer1, and Timer2) to
 * generate PWM signals on specific pins (PD6, PD5, PB1, PB2, PB3). It sets up the
 * timers in Fast PWM mode with different prescalers and enables interrupts for
 * each timer overflow.
 */
void PWM_Init()
{
    DDRD = (1 << PORTD6) | (1 << PORTD5); /**< Set PD6, PD5 as output. */
    DDRB = (1 << PORTB1) | (1 << PORTB2) | (1 << PORTB3); /**< Set PB1, PB2, PB3 as output. */

    // Configure Timer0 (8-bit Fast PWM)
    TCCR0A = (1 << WGM00) | (1 << WGM01) | (1 << COM0A1) | (1 << COM0B1);
    TCCR0B = (1 << CS01); /**< Prescaler = 8 */

    // Configure Timer1 (16-bit Fast PWM)
    TCCR1A = (1 << WGM10) | (1 << COM1A1) | (1 << COM1B1);
    TCCR1B = (1 << WGM12) | (1 << CS11) | (1 << CS10); /**< Prescaler = 64 */

    // Configure Timer2 (8-bit Fast PWM)
    TCCR2A = (1 << WGM20) | (1 << WGM21) | (1 << COM2A1);
    TCCR2B = (1 << CS22); /**< Prescaler = 64 */

    // Enable Timer overflow interrupts
    TIMSK0 = (1 << TOIE0);
    TIMSK1 = (1 << TOIE1);
    TIMSK2 = (1 << TOIE2);

    sei(); /**< Enable global interrupts. */
}

/**
 * @brief Updates the PWM duty cycles based on the dutycycle array.
 *
 * This function updates the OCR (Output Compare Register) values for each timer,
 * effectively changing the duty cycle of the PWM signal. The duty cycle is mapped
 * from the `dutycycle` array (values between 0 and 100) to the range of the timers
 * (0 to 255).
 */
void update_PWM()
{
    OCR0A = (dutycycle[0] * MAX_VALUE) / MAXDC; /**< PD6 */
    OCR0B = (dutycycle[1] * MAX_VALUE) / MAXDC; /**< PD5 */
    OCR1A = (dutycycle[2] * MAX_VALUE) / MAXDC; /**< PB1 */
    OCR1B = (dutycycle[3] * MAX_VALUE) / MAXDC; /**< PB2 */
    OCR2A = (dutycycle[4] * MAX_VALUE) / MAXDC; /**< PB3 */
}

/**
 * @brief Main program loop.
 *
 * The main loop checks if any interrupts were triggered. If an interrupt has been
 * received, it increases the duty cycle of each PWM channel by 10%, looping back
 * to 0% once the maximum (100%) is reached. It then updates the PWM signals and
 * waits for 100 milliseconds before checking again.
 */
int main(void)
{
    PWM_Init();

    while (1)
    {
        for (uint8_t i = 0; i < 3; i++)
        {
            if (intrpt_rcvd[i])
            {
                for (uint8_t j = 0; j < 5; j++)
                {
                    dutycycle[j] += 10;
                    if (dutycycle[j] > MAXDC)
                        dutycycle[j] = MINDC;
                }
                intrpt_rcvd[i] = false;
            }
        }

        update_PWM();

        _delay_ms(100);
    }

    return 0;
}

/**
 * @brief Timer0 Overflow Interrupt Service Routine.
 *
 * This ISR sets the flag indicating that Timer0 has overflowed, triggering a change
 * in the PWM duty cycle.
 */
ISR(TIMER0_OVF_vect)
{
    intrpt_rcvd[0] = true;
}

/**
 * @brief Timer1 Overflow Interrupt Service Routine.
 *
 * This ISR sets the flag indicating that Timer1 has overflowed, triggering a change
 * in the PWM duty cycle.
 */
ISR(TIMER1_OVF_vect)
{
    intrpt_rcvd[1] = true;
}

/**
 * @brief Timer2 Overflow Interrupt Service Routine.
 *
 * This ISR sets the flag indicating that Timer2 has overflowed, triggering a change
 * in the PWM duty cycle.
 */
ISR(TIMER2_OVF_vect)
{
    intrpt_rcvd[2] = true;
}

/**
 * @file main.c
 * @brief ADC to PORTD using ATmega
 * @details This program reads the ADC value from ADC0 and outputs it to PORTD.
 *
 * @author kandyala sai kumar
 * @date 2025
 */

/** Define CPU frequency */
#define F_CPU 16000000UL

/** Include standard AVR and utility libraries */
#include <avr/io.h>
#include <util/delay.h>

/** @brief Main function
 *  @return int Always returns 0
 */
int main(void)
{
    /** Configure PORTD as output (All pins set as output) */
    DDRD = 0xFF;  /* MISRA Rule: Directly assigning values to registers should be done cautiously */

    /** Select ADC0 channel (MUX[3:0] = 0000) */
    ADMUX = 0x00; /* Reference voltage is set to AREF (default) */

    /** ADC CONTROL AND STATUS REGISTER FOR ENABLE ADC CONVERSION */
    ADCSRA = 0x87; /* ADC Enable */

    while(1)
    {
        /** Start ADC conversion */
        ADCSRA |= (1 << ADSC);

        /** Wait until conversion is complete (ADSC bit becomes 0) */
        while ((ADCSRA & (1 << ADSC)) != 0)
        {
            /* Do nothing, just wait for ADC conversion to complete */
        }

        /** Store the ADC result (lower 8 bits) in PORTD */
        PORTD = (uint8_t)(ADC & 0xFF); /* MISRA: Explicit typecasting applied */
    }

    return 0;
}

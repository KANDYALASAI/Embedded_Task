/**
 * @file main.c
 * @brief ADC-based PWM control for AVR microcontroller
 *
 * This program reads an analog voltage using the ADC and adjusts the PWM duty cycle
 * based on the input voltage.
 *
 * @author kandyala sai kumar
 * @date 2025-01-08
 */

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdbool.h>
#include <avr/interrupt.h>

#define DIV_FACTOR ((float)100.0) ///< Factor for duty cycle calculation
#define MAX_VALUE  ((float)255.0) ///< Maximum OCR0A value
#define DELAY_MS    10000 ///< Delay in milliseconds
#define DCCINCFCTR  20 ///< Duty cycle increment factor
#define MAXDC       100 ///< Maximum duty cycle
#define MINDC        0 ///< Minimum duty cycle

/**
 * @brief Global variables for ADC and PWM
 */
int dutycycle; ///< Duty cycle percentage
volatile bool intrpt_rcvd = false; ///< Interrupt received flag
int adc_value; ///< ADC read value
float voltage_value; ///< Converted voltage value

/**
 * @brief Initializes ADC module
 */
void ADC_init(void);

/**
 * @brief Initializes PWM using Timer0
 */
void PWM_init(void);

/**
 * @brief Reads ADC and updates PWM duty cycle
 */
void ADC_PWM(void);

/**
 * @brief Reads ADC value
 * @return int ADC read value
 */
int Read_ADC(void);

/**
 * @brief Converts ADC value to voltage
 * @param adcinput ADC read value
 * @return int Converted voltage value
 */
int Convert_voltage(int adcinput);

/**
 * @brief Generates PWM based on voltage input
 * @param voltageinput Voltage read from ADC
 * @return float PWM duty cycle percentage
 */
float generate_PWM(float voltageinput);

int main(void)
{
    ADC_init();
    PWM_init();
    while(1)
    {
        ADC_PWM();
    }
    return 0;
}

void ADC_init(void)
{
   DDRD = 0xFF; ///< Set Port D as output
   ADMUX = 0x00; ///< Select ADC0 channel
   ADCSRA = 0x87; ///< Enable ADC and set prescaler
}

void PWM_init(void)
{
    DDRD = (1<<PORTD6); ///< Set PD6 as output
    TCCR0A = (1<<WGM00) | (1<<WGM01) | (1<<COM0A1); ///< Fast PWM mode
    TIMSK0 = (1<<TOIE0); ///< Enable Timer0 overflow interrupt
    OCR0A = ((dutycycle / DIV_FACTOR) * MAX_VALUE);
    sei(); ///< Enable global interrupts
    TCCR0B = (1<<CS00); ///< Start Timer0 with no prescaler
}

int Convert_Voltage(int adcinput)
{
    voltage_value = (adcinput / 1024.0) * 5.0;
    return voltage_value;
}

void ADC_PWM(void)
{
    adc_value = Read_ADC();
    voltage_value = Convert_Voltage(adc_value);
    generate_PWM(voltage_value);
}

int Read_ADC(void)
{
    ADCSRA |= (1<<ADSC); ///< Start ADC conversion
    while(ADCSRA & (1<<ADSC)); ///< Wait for conversion to complete
    return ADC;
}

float generate_PWM(float voltageinput)
{
    if (intrpt_rcvd == true)
    {
        if (voltage_value == 5.0)
        {
            dutycycle = 100;
        }
        else if (voltage_value >= 4.0)
        {
            dutycycle = 75;
        }
        else if (voltage_value >= 3.0)
        {
            dutycycle = 50;
        }
        else if (voltage_value >= 2.0)
        {
            dutycycle = 25;
        }
        else
        {
            dutycycle = 0;
        }
        intrpt_rcvd = false; ///< Reset the flag
        OCR0A = ((dutycycle / DIV_FACTOR) * MAX_VALUE);
    }
    return dutycycle;
}

/**
 * @brief Timer0 overflow interrupt service routine
 */
ISR(TIMER0_OVF_vect)
{
    intrpt_rcvd = true; ///< Set interrupt received flag
}

#pragma config FOSC = EXTRC     // Oscillator Selection bits (RC oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable bit (BOR disabled)
#pragma config LVP = OFF        // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

#define _XTAL_FREQ 20000000     // Define the operating frequency of the microcontroller (20 MHz)
#include <xc.h>

// Function to set PWM duty cycle
void setpwmdutycycle(unsigned int dutycycle)
{
    CCPR1L = (dutycycle >> 2);               // Assign the higher 8 bits to CCPR1L
    CCP1CON = (CCP1CON & 0xCF) | ((dutycycle & 0x03) << 4); // Assign the lower 2 bits to CCP1CON<5:4>
}

void main(void)
{
    int i = 0;
    TRISC = 0x00;          // Configure PORTC as output
    CCP1CON = 0x0C;        // Configure CCP1 module in PWM mode
    PR2 = 0xFF;            // Set the PWM period
    T2CON = 0x04;          // Enable Timer2 with a prescaler of 1

    while (1)
    {
        // Increase the duty cycle gradually
        for (i = 0; i < 1023; i++)
        {
            setpwmdutycycle(i);
            __delay_ms(50);
        }

        // Decrease the duty cycle gradually
        for (i = 1023; i > 0; i--)
        {
            setpwmdutycycle(i);
            __delay_ms(50);
        }
    }
    return;
}

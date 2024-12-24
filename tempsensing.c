#pragma config FOSC = EXTRC     // Oscillator Selection bits (RC oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable bit (BOR disabled)
#pragma config LVP = OFF        // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

#include <xc.h>
#define _XTAL_FREQ 20000000

#define RS PORTDbits.RD0
#define RW PORTDbits.RD1
#define EN PORTDbits.RD2

volatile unsigned int adc = 0; // ADC result
float adc1 = 0, temp = 0;
int a, b, c, d, e, f;

// Function prototypes
void lcd_data(unsigned char data);
void lcd_command(unsigned char cmd);
void lcd_string(const unsigned char *str, unsigned char num);
void lcd_init();

void __interrupt() interrupt_adc()
{
    if (PIR1bits.ADIF)
    {
        adc = (ADRESH << 8) + ADRESL; // Combine high and low bytes
        PIR1bits.ADIF = 0;            // Clear interrupt flag
    }
}

void lcd_data(unsigned char data)
{
    PORTC = data;
    RS = 1;
    RW = 0;
    EN = 1;
    __delay_ms(5);
    EN = 0;
}

void lcd_command(unsigned char cmd)
{
    PORTC = cmd;
    RS = 0;
    RW = 0;
    EN = 1;
    __delay_ms(5);
    EN = 0;
}

void lcd_string(const unsigned char *str, unsigned char num)
{
    for (unsigned char i = 0; i < num; i++)
    {
        lcd_data(str[i]);
    }
}

void lcd_init()
{
    lcd_command(0x38); // 8-bit mode, 2-line display, 5x8 font
    lcd_command(0x06); // Entry mode: auto-increment cursor
    lcd_command(0x0C); // Display ON, Cursor OFF
    lcd_command(0x01); // Clear display
}

void main(void)
{
    // Configure ports and peripherals
    TRISC = 0x00; // Set PORTC as output (LCD data lines)
    TRISD = 0x00; // Set PORTD as output (LCD control lines)
    PORTC = PORTD = 0x00;

    // ADC configuration
    ADCON0 = 0x41; // ADC enabled, select channel AN0
    ADCON1 = 0x8E; // Configure Vref and analog channels

    // Interrupts configuration
    INTCONbits.GIE = 1;   // Enable global interrupts
    INTCONbits.PEIE = 1;  // Enable peripheral interrupts
    PIE1bits.ADIE = 1;    // Enable ADC interrupt

    lcd_init(); // Initialize the LCD

    lcd_command(0x80); // Set cursor to the first line
    lcd_string("TEMP:", 5); // Display "TEMP:"

    while (1)
    {
        ADCON0 |= 0x04; // Start ADC conversion
        __delay_ms(50); // Wait for conversion to complete

        adc1 = adc / 2.046;
        temp = adc1 * 100;

        lcd_command(0x89); // Set cursor position
        a = (int)temp / 10;
        b = (int)temp % 10;
        c = a % 10;
        d = a / 10;
        e = d % 10;
        f = d / 10;

        lcd_data(f + 0x30);
        lcd_data(e + 0x30);
        lcd_data('.');
        lcd_data(c + 0x30);
        lcd_data(b + 0x30);

        __delay_ms(20); // Refresh rate
    }
}

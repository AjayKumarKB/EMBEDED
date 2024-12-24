#pragma config FOSC = EXTRC     // Oscillator Selection bits (RC oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable bit (BOR disabled)
#pragma config LVP = OFF        // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

#define RS PORTDbits.RD0
#define RW PORTDbits.RD1
#define EN PORTDbits.RD2
#define C1 PORTBbits.RB0
#define C2 PORTBbits.RB1
#define C3 PORTBbits.RB2
#define R1 PORTBbits.RB4
#define R2 PORTBbits.RB5
#define R3 PORTBbits.RB6
#define R4 PORTBbits.RB7
#define _XTAL_FREQ 20000000

#include <xc.h>

// Function to send data to the LCD
void lcd_data(unsigned char data) 
{
    PORTC = data;
    RS = 1;
    RW = 0;
    EN = 1;
    __delay_ms(5);
    EN = 0;
}

// Function to send a command to the LCD
void lcd_command(unsigned char cmd)
{
    PORTC = cmd;
    RS = 0;
    RW = 0;
    EN = 1;
    __delay_ms(5);
    EN = 0;
}

// Function to display a string on the LCD
void lcd_string(const unsigned char *str, unsigned char num)
{
    unsigned char i;
    for (i = 0; i < num; i++)
    {
        lcd_data(str[i]);
    }
}

// Initialize the LCD
void lcd_init()
{
    lcd_command(0x38);  // 8-bit mode, 2-line display, 5x8 font
    lcd_command(0x06);  // Entry mode: auto-increment cursor
    lcd_command(0x0C);  // Display ON, Cursor OFF
    lcd_command(0x01);  // Clear display
}

// Function to handle keypad input
void keypad() 
{
    // Column 1
    C1 = 1; C2 = 0; C3 = 0;
    if (R1 == 1) 
    {
        lcd_data('1');
        __delay_ms(200);
        while (R1 == 1);
    }
    if (R2 == 1)
    {
        lcd_data('4');
        __delay_ms(200);
        while (R2 == 1);
    }
    if (R3 == 1)
    {
        lcd_data('7');
        __delay_ms(200);
        while (R3 == 1);
    }
    if (R4 == 1) 
    {
        lcd_data('*');
        __delay_ms(200);
        while (R4 == 1);
    }

    // Column 2
    C1 = 0; C2 = 1; C3 = 0;
    if (R1 == 1)
    {
        lcd_data('2');
        __delay_ms(200);
        while (R1 == 1);
    }
    if (R2 == 1) 
    {
        lcd_data('5');
        __delay_ms(200);
        while (R2 == 1);
    }
    if (R3 == 1)
    {
        lcd_data('8');
        __delay_ms(200);
        while (R3 == 1);
    }
    if (R4 == 1)
    {
        lcd_data('0');
        __delay_ms(200);
        while (R4 == 1);
    }

    // Column 3
    C1 = 0; C2 = 0; C3 = 1;
    if (R1 == 1)
    {
        lcd_data('3');
        __delay_ms(200);
        while (R1 == 1);
    }
    if (R2 == 1)
    {
        lcd_data('6');
        __delay_ms(200);
        while (R2 == 1);
    }
    if (R3 == 1)
    {
        lcd_data('9');
        __delay_ms(200);
        while (R3 == 1);
    }
    if (R4 == 1) 
    {
        lcd_data('#');
        __delay_ms(200);
        while (R4 == 1);
    }

    // Reset columns to zero
    C1 = 0; C2 = 0; C3 = 0;
}

void main(void) {
    TRISC = 0x00;  // Set PORTC as output (LCD data lines)
    TRISD = 0x00;  // Set PORTD as output (LCD control lines)
    TRISB = 0xF0;  // Set upper nibble of PORTB as input (keypad rows), lower nibble as output (keypad columns)

    lcd_init();  // Initialize the LCD

    lcd_command(0x80);  // Set cursor to the first line
    lcd_string("KEYPAD:", 7);  // Display "KEYPAD:"
    lcd_command(0xC0);  // Move to the second line

    while (1) {
        keypad();  // Continuously check for keypad input
    }

    return;
}

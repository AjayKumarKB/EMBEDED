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
#define AJAY 7  // Value to write to EEPROM
#define _XTAL_FREQ 20000000

#include <xc.h>

unsigned char byte;

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
    __delay_ms(20);     // Delay for LCD to process
}

// Read a byte from EEPROM
void readbyte(const unsigned char address)
{
    EEADR = address;
    EECON1bits.EEPGD = 0;  // Access data memory
    EECON1bits.RD = 1;     // Start read
    byte = EEDATA;
}

// Write a byte to EEPROM
void writebyte(unsigned char data, unsigned char address)
{
    EEADR = address;
    EEDATA = data;
    EECON1bits.EEPGD = 0;  // Access data memory
    EECON1bits.WREN = 1;   // Enable write
    EECON2 = 0x55;         // Required write sequence
    EECON2 = 0xAA;
    EECON1bits.WR = 1;     // Start write
    while (PIR2bits.EEIF == 0);  // Wait for completion
    PIR2bits.EEIF = 0;     // Clear interrupt flag
    EECON1bits.WREN = 0;   // Disable write
}

void main(void) {
    TRISC = 0x00;  // Set PORTC as output (LCD data lines)
    TRISD = 0x00;  // Set PORTD as output (LCD control lines)
    PORTC = 0x00;

    lcd_init();  // Initialize the LCD

    lcd_command(0x80);  // Set cursor to the first line
    lcd_string("EEPROM:", 7);  // Display "EEPROM:"
    lcd_command(0xC0);  // Move to the second line

    while (1)
    {
        writebyte(7, 0xFF);  // Write AJAY (7) to EEPROM address 0x0A
        readbyte(0xFF);         // Read from EEPROM address 0x0A
        lcd_command(0xC0);      // Move to the second line
        lcd_data(byte + 0x30);  // Display the read byte as a character
        __delay_ms(1000);       // Add delay for visibility
    }

    return;
}

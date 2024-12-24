#include <xc.h>

// Configuration bits for PIC16F877A (set for internal oscillator, no watchdog timer)

#pragma config WDTE = OFF               // Watchdog Timer Disable
#pragma config PWRTE = OFF              // Power-up Timer Disable
#pragma config BOREN = OFF              // Brown-out Reset Disable
#pragma config LVP = OFF                // Low-Voltage Programming Disable
#pragma config CPD = OFF                // Data EEPROM Code Protection Disable
#pragma config WRT = OFF                // Flash Program Memory Write Enable (disable)

#define _XTAL_FREQ 4000000  // Set the internal clock frequency (4 MHz)

void main(void) {
    TRISB = 1;  // Set RB0 as input for push button
    TRISC = 0;  // Set RC0 as output for LED
    
    RC0 = 0;      // Initially turn off the LED
    
    while(1) {
        // Check if the push button is pressed (low logic level when pressed)
        if (RB0 == 0) {
            // If the button is pressed, turn on the LED
            RC0 = 1;
            TRISC=1;
        } else {
            // If the button is not pressed, turn off the LED
            RC0 = 0;
        }
    }
}

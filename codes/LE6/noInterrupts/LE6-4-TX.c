#include <xc.h>
#include <stdint.h>

// Configuration Bits
#pragma config FOSC = XT
#pragma config WDTE = OFF
#pragma config PWRTE = ON
#pragma config BOREN = ON
#pragma config LVP = OFF
#pragma config CPD = OFF
#pragma config WRT = OFF
#pragma config CP = OFF

#define _XTAL_FREQ 4000000  // 4 MHz crystal

// USART Initialization
void initUSART_TX(void) {
    TRISC6 = 0;  // TX as output
    TRISC7 = 1;  // RX as input (not used in TX role)

    SPBRG = 25;  // Baud rate 9600 at 4 MHz with BRGH = 1
    BRGH = 1;
    SYNC = 0;
    SPEN = 1;
    TXEN = 1;
}

// Transmit a single character
void sendChar(char ch) {
    while (!TXIF);  // Wait until TXREG is ready
    TXREG = ch;
}

// Delay function to debounce
void delay(int mul) {
    int i, j;
    for (i = mul; i != 0; i--) {
        for (j = 0; j < 255; j++);
    }
}

void main(void) {
    initUSART_TX();
    TRISB = 0xFF;  // PORTB as input
    PORTB = 0x11;

    while (1) {
        while (!RB4);  // Wait for a key press (RD4 = Data Available)
        
        uint8_t key = PORTB & 0x0F;  // Read lower 4 bits

        switch (key) {
            case 0x00: sendChar('1'); break;
            case 0x01: sendChar('2'); break;
            case 0x02: sendChar('3'); break;
            case 0x04: sendChar('4'); break;
            case 0x05: sendChar('5'); break;
            case 0x06: sendChar('6'); break;
            case 0x08: sendChar('7'); break;
            case 0x09: sendChar('8'); break;
            case 0x0A: sendChar('9'); break;
            case 0x0D: sendChar('0'); break;
            case 0x0C: sendChar('*'); break;
            case 0x0E: sendChar('#'); break;
            default: break;
        }

        while (RB4);   // Wait until key is released
        delay(10);     // Debounce
    }
}

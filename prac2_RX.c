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

// Function to initialize USART for receiving
void initUSART(void) {
    TRISC7 = 1;  // RX as input
    TRISC6 = 0;  // TX as output (not used in RX, but good practice)

    SPBRG = 25;  // Baud rate 9600 @ 4MHz with BRGH = 1
    BRGH = 1;
    SYNC = 0;    // Asynchronous mode
    SPEN = 1;    // Enable serial port
    CREN = 1;    // Enable continuous reception
}

// Function to received character to 7-segment code
uint8_t decodeTo7Segment(char c) {
    switch (c) {
        case '0': return 0x00;  // 0
        case '1': return 0x01;  // 1
        case '2': return 0x02;  // 2
        case '3': return 0x03;  // 3
        case '4': return 0x04;  // 4
        case '5': return 0x05;  // 5
        case '6': return 0x06;  // 6
        case '7': return 0x07;  // 7
        case '8': return 0x08;  // 8
        case '9': return 0x09;  // 9
        default:   return 0x00;  // Default to blank for unknown
    }
}

void main(void) {
    initUSART();
    TRISB = 0x00;  // Set PORTB as output for 7-segment display
    PORTB = 0x00;  // Initialize display to blank
    TRISD = 0x00;
    PORTD = 0x00;

    char receivedChar;

    while (1) {
        // if (RCIF) { 
        //     receivedChar = RCREG;              // Read received character
        //     PORTB = receivedChar; 
            
        //     // PORTD = decodeTo7Segment(receivedChar);
        // }
        if(RCIF)
        {
	        receivedChar = RCREG;              // Read received character
        	PORTB = receivedChar;
        	 while(!RCIF);
	        receivedChar = RCREG;              // Read received character
	        PORTD = receivedChar;
        }
        

       
    }
}
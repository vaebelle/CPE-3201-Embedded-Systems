#include <xc.h>

// Configuration bits
#pragma config FOSC = XT
#pragma config WDTE = OFF
#pragma config PWRTE = ON
#pragma config BOREN = ON
#pragma config LVP = OFF
#pragma config CPD = OFF
#pragma config WRT = OFF
#pragma config CP = OFF


// Initialize USART (9600 baud, 4 MHz, asynchronous)
void initUSART(void) {
    TRISC6 = 0;  // TX as output
    TRISC7 = 1;  // RX as input

    SPBRG = 25;  // Baud rate generator for 9600 baud (BRGH = 1)
    BRGH = 1;

    SYNC = 0;    // Asynchronous mode
    SPEN = 1;    // Enable serial port
    TXEN = 1;    // Enable transmitter
    CREN = 1;    // Enable receiver
}

// Custom delay routine (~10ms steps)
void delay(int MUL) {
    int i, j;
    for (i = MUL; i != 0; i--) {
        for (j = 0; j < 256; j++) {
            // Software delay loop
        }
    }
}

// Send a single character over USART
void sendChar(char message) {
    while (!TXIF);   // Wait until TXREG is ready
    TXREG = message;
}

// Send a string followed by a newline
void sendString(const char *s) {
    while (*s) {
        sendChar(*s++);
    }
    sendChar('\r');  // Carriage return
    sendChar('\n');  // New line
}

// Debounce function for RB4 (active low)
void debounce(void) {
    while (RB4);      // Wait for key press
    delay(10);        // Debounce press
    while (!RB4);     // Wait for key release
    delay(10);        // Debounce release
}

void main(void) {
    initUSART();
    TRISB = 0xFF;  // Set PORTB as input (74C922 output)
    PORTB = 0x11;

    int key;

    while (1) {
        if (!RB4) {
            debounce();            
            key = PORTB & 0x0F;       // Mask to read D0–D3 (key code)

            switch (key) {
                case 0x00:
                    sendString("You pressed 1.");
                    break;
                case 0x01:
                    sendString("You pressed 2.");
                    break;
                case 0x02:
                    sendString("You pressed 3.");
                    break;
                case 0x04:
                    sendString("You pressed 4.");
                    break;
                case 0x05:
                    sendString("You pressed 5.");
                    break;
                case 0x06:
                    sendString("You pressed 6.");
                    break;
                case 0x08:
                    sendString("You pressed 7.");
                    break;
                case 0x09:
                    sendString("You pressed 8.");
                    break;
                case 0x0A:
                    sendString("You pressed 9.");
                    break;
                case 0x0C:
                    sendString("You pressed *.");
                    break;
                case 0x0D:
                    sendString("You pressed 0.");
                    break;
                case 0x0E:
                    sendString("You pressed #.");
                    break;
                default:
                    break;
            }
        }
    }
}

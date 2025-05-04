#include <xc.h>
#include <stdio.h>

#pragma config FOSC = XT
#pragma config WDTE = OFF
#pragma config PWRTE = ON
#pragma config BOREN = ON
#pragma config LVP = OFF
#pragma config CPD = OFF
#pragma config WRT = OFF
#pragma config CP = OFF

unsigned int overflowCount = 0;
unsigned int secondTmrFlag = 0;
int counter = 0;
char buffer[6];
int DATA;
int mode = 0;

void interrupt ISR()
{
    GIE = 0;
    // Only disable interrupts when necessary
    if (INTF) {
        INTF = 0; // clear interrupt flag
        DATA = PORTC & 0x0F; // Read data from PORTC
        
        if (DATA == 0x0C) {  // count up
            mode = 1;
        }
        else if (DATA == 0x0D) { // zero
            mode = 2;
        }
        else if (DATA == 0x0E) { // count down
            mode = 3;
        }
    }
    else if (TMR1IF) {
        TMR1IF = 0; // clear the Timer1 interrupt flag
        TMR1H = 0x0B;
        TMR1L = 0xDC; // reloading Timer1
        
        overflowCount++;
        
        if (overflowCount >= 2) {
            overflowCount = 0;
            secondTmrFlag = 1;
        }
    }

    GIE = 1;
}

void delay(int MUL) 
{
    int i, j;
    for (i = MUL; i != 0; i--) {
        for (j = 0; j < 256; j++) {}
    }
}

void delayLCD(int MUL) 
{
    int i, j;
    for (i = MUL; i != 0; i--) {
        for (j = 0; j < 256; j++) {}
    }
}

void instCtrl(unsigned char INST) 
{
    PORTD = INST; // load instruction to PORTD
    RC4 = 0;      // set RS to 0 (instruction register)
    RC5 = 0;      // set RW to 0 (write)
    RC6 = 1;      // set E to 1
    delayLCD(1);  // 1 ms delay (estimate)
    RC6 = 0;      // set E to 0 (strobe)
}

void dataCtrl(unsigned char DATA) 
{
    PORTD = DATA; // load data to PORTD
    RC4 = 1;      // set RS to 1 (data register)
    RC5 = 0;      // set RW to 0 (write)
    RC6 = 1;      // set E to 1
    delayLCD(1);  // 1 ms delay (estimate)
    RC6 = 0;      // set E to 0 (strobe)
}

void initLCD() 
{
    delayLCD(1);    // LCD startup about approx 15ms
    instCtrl(0x38); // function set: 8-bit; dual-line
    instCtrl(0x08); // display off
    instCtrl(0x01); // display clear
    instCtrl(0x06); // entry mode: increment; shift off
    instCtrl(0x0C); // display on; cursor off; blink off
}

void displayString(const char *input) 
{
    int i;
    for (i = 0; input[i] != '\0'; i++) {
        dataCtrl(input[i]);
    }
}

void initADC() 
{
    ADCON1 = 0x80; // Configure all pins as digital
    ADCON0 = 0x41; // ADC on, channel 0 (AN0)
}

unsigned int readADC() 
{
    ADCON0bits.GO = 1; // Start conversion
    while (ADCON0bits.GO); // Wait for conversion to finish
    return (ADRESH << 8) | ADRESL; // Return the 10-bit result
}

void voltageReading(){
    unsigned int adcValue = readADC();
    float voltage = adcValue * (5.0 / 1023.0); // Assuming 5V reference
    sprintf(buffer, "%.2f V", voltage); // Format voltage as string
    instCtrl(0x9D); // Move to new position on LCD
    displayString(buffer);
}

void main() {
    // Port initializations
    TRISB = 0xFF; // Input
    TRISC = 0x0F; // Input
    TRISD = 0x00; // Output

    // Interrupt initializations
    INTEDG = 1;   // Interrupt at rising edge
    INTE = 1;     // Enable RB0/INT external interrupt
    INTF = 0;     // Clears the interrupt flag

    // Timer 1 initialization
    T1CON = 0x30;  // 1:8 prescaler, internal clock, Timer1 off
    TMR1IE = 1;    // Enable Timer1 overflow interrupt (PIE1 reg)
    PEIE = 1;      // Enable all peripheral interrupt (INTCON reg)
    GIE = 1;       // Enable all unmasked interrupts (INTCON reg)
    TMR1H = 0x0B;
    TMR1L = 0xDC;
    TMR1ON = 1;    // Turns on timer 1

    initLCD();
    initADC();

    while (1) {
        instCtrl(0x80);
        displayString("Counter: ");

        instCtrl(0xC0);
        displayString("Mode: "); // count up or count down

        instCtrl(0x94);
        displayString("Voltage: "); // ADC voltage reading every 500ms

        if (mode == 1) { // COUNT UP
            if (secondTmrFlag == 1) {
                secondTmrFlag = 0;
                counter++;
                sprintf(buffer, "%d", counter); // Convert int to string
                instCtrl(0x8A); // Move cursor to desired position (e.g., row 1, col 10)
                displayString(buffer); // Display the string on the LCD
                instCtrl(0xC9);
                for (int i = 10; i < 20; i++) 
                {
                    dataCtrl(0x20);  // Write a space character (0x20) to clear each position
                }
                instCtrl(0xC9);
                displayString("COUNT UP");

                voltageReading();
            }
        }

        else if (mode == 2) { // Zero
            counter = 0;
            sprintf(buffer, "%d", counter); // Convert int to string
            instCtrl(0x8A); // Move cursor to desired position
            displayString(buffer);
            instCtrl(0xC9);
            for (int i = 10; i < 20; i++) 
            {
                dataCtrl(0x20);  // Write a space character (0x20) to clear each position
            }
            instCtrl(0xC9);
            displayString("ZERO");

            voltageReading();
        }

        else if (mode == 3) { // COUNT DOWN
            if (secondTmrFlag == 1) {
                secondTmrFlag = 0;
                counter--;
                sprintf(buffer, "%d", counter); // Convert int to string
                instCtrl(0x8A); // Move cursor to desired position
                displayString(buffer); // Display the string on the LCD
                instCtrl(0xC9);
                for (int i = 10; i < 20; i++) 
                {
                    dataCtrl(0x20);  // Write a space character (0x20) to clear each position
                }
                instCtrl(0xC9);
                displayString("COUNT DOWN");

                voltageReading();
            }
        }

    }

    /*
    
    Row 1 starts at address 0x00 and ends at 0x13 (0 to 19).

    Row 2 starts at address 0x40 and ends at 0x53 (40 to 59).

    Row 3 starts at address 0x14 and ends at 0x27 (20 to 39).

    Row 4 starts at address 0x54 and ends at 0x67 (60 to 79).
    */
}

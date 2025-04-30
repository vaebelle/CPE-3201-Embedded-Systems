/*
Author:     Bea Belle Therese B. Caños
Course:     CpE 3201
Activity:   LE4-6 | Displaying Period
*/

#include <xc.h>

#pragma config FOSC = XT
#pragma config WDTE = OFF
#pragma config PWRTE = ON
#pragma config BOREN = ON
#pragma config LVP = OFF
#pragma config CPD = OFF
#pragma config WRT = OFF
#pragma config CP = OFF

// Global Variables
bit CCP1IF_flag = 0;
unsigned long int period = 0;
unsigned char periodDisp[6]; 

void interrupt ISR()
{
    GIE = 0; 

    if (CCP1IF) {
        CCP1IF = 0;  
        TMR1 = 0;    

        period = CCPR1 / 1000;  
        period = period * 8;    

        CCP1IF_flag = 1; 

    }

    GIE = 1; 
}

void delayLCD(int MUL) {
    int i, j;
    for (i = MUL; i != 0; i--) {
        for (j = 0; j < 256; j++) {} 
    }
}

void instCtrl (unsigned char INST)
{
	PORTB = INST; // load instruction to PORTB
	RD0 = 0; // set RS to 0 (instruction reg)
	RD1 = 0; // set RW to 0 (write)
	RD2 = 1; // set E to 1
	delayLCD(1); // 1 ms delay (estimate)
	RD2 = 0; // set E to 0 (strobe)
}

void dataCtrl (unsigned char DATA)
{
	PORTB = DATA; 		// load data to PORTB
	RD0 = 1; 			// set RS to 1 (data reg)
	RD1 = 0; 			// set RW to 0 (write)
	RD2 = 1; 			// set E to 1
	delayLCD(1); 			// 1 ms delay (estimate)
	RD2 = 0; 			// set E to 0 (strobe)
}

void initLCD()
{
    delayLCD(15);       // Wait at least 15ms after power-on
    instCtrl(0x38);     // Function set: 8-bit, 2-line, 5x8 font
    instCtrl(0x08);     // Display off
    instCtrl(0x01);     // Clear display
    delayLCD(2);        // Wait for display to clear (~1.53ms)
    instCtrl(0x06);     // Entry mode: increment cursor, no shift
    instCtrl(0x0E);     // Display on, cursor on, blink off
}

void display(const char *input)
{
    int i;
    for (i = 0; input[i] != '\0'; i++) {
        dataCtrl(input[i]);
    }
}

void intToASCII(unsigned int num, char *str) {
    int i = 0;
    char temp[6]; 

    if (num == 0) {
        str[i] = '0';
        i++;
    } 
    else {
        while (num > 0) {
            temp[i] = (num % 10) + '0'; // Convert digit to ASCII
            i++;
            num /= 10;
    	}
    }

    // Reverse the number string
    int j;
    for (j = 0; j < i; j++) {
        str[j] = temp[i - j - 1];
    }
    str[i] = '\0'; // Null terminator
}

void displayPeriod(unsigned int period) {
    instCtrl(0x80); 
    instCtrl(0x01); 
    delayLCD(2);        // Delay after clear display 
    
    intToASCII(period, periodDisp); 
    display(periodDisp); 

	delayLCD(10);
}

void main() {
    // Configure ports
    TRISA = 0x00;
    TRISB = 0x00;
    TRISC = 0x04; // Set RC2 as input (CCP1)
    TRISD = 0x00;
    ADCON1 = 0x06;

    initLCD();

    PORTA = 0x00;

    // Timer1 and CCP Configuration
    T1CON = 0x30;     // 1:8 prescaler, Timer1 OFF initially
    CCP1CON = 0x05;   // Capture mode: every rising edge
    CCP1IE = 1;       // Enable CCP1 interrupt
    CCP1IF = 0;       // Clear interrupt flag
    PEIE = 1;         // Enable peripheral interrupts
    GIE = 1;          // Enable global interrupts
    TMR1ON = 1;       // Start Timer1

    while (1) {

        if (CCP1IF_flag) {
            displayPeriod(period); // Update LCD
            CCP1IF_flag = 0; // Reset flag after displaying
        }
    }
}

#include <xc.h>

#pragma config FOSC = XT
#pragma config WDTE = OFF
#pragma config PWRTE = ON
#pragma config BOREN = ON
#pragma config LVP = OFF
#pragma config CPD = OFF
#pragma config WRT = OFF
#pragma config CP = OFF

volatile bit update_display = 0;  // Flag for display update

// Interrupt Service Routine
void interrupt ISR() {
    if (TMR0IF) { 
        TMR0IF = 0;             
        update_display = 1;  
        TMR0 = 100;
    }
}

void delay(int MUL)
{
    int i, j;
    for (i = MUL; i != 0; i--)
    {
        for (j = 0; j < 256; j++)
        {
        }
    } // loop until i = 0
}

// Read ADC value from AN0
int readADC (void)
{
    int temp = 0;
    delay(1);
    GO = 1;

    while (GO_DONE == 1);

    temp = ADRESH;
    temp = temp << 8;
    temp = temp | ADRESL;
    return temp;
}

// Convert ADC value to voltage and display on PORTB
void displayVoltage(unsigned int adc_value) {
    unsigned int voltageVal = ((adc_value * 50) + 511) / 1023;  // Rounding
    unsigned char whole_number = voltageVal / 10;   // Extract integer part
    unsigned char decimal_digit = voltageVal % 10;    // Extract first decimal digit
    
    PORTB = (whole_number << 4) | decimal_digit;    // Assign upper and lower nibbles
}

void main() {
    TRISB = 0x00;  // Set PORTB as output
    PORTB = 0x00;  // Clear PORTB
    ADCON1 = 0x80; // Right-justified result, Vref+ = VDD (5V), Vref- = VSS (0V)
    ADCON0 = 0x41; // Select AN0, ADC enabled

    // Configure Timer0 for periodic updates
    OPTION_REG = 0x07;      
    TMR0 = 100;             // Initial Timer0 value
    TMR0IE = 1;             // Enable Timer0 interrupt
    GIE = 1;                // Enable global interrupts

    while (1) {
        if (update_display) {  // Check if it's time to update
            update_display = 0;  // Reset flag
            unsigned int adc_value = readADC();  // Read ADC
            displayVoltage(adc_value);  // Update 7-segment display
        }
    }
}

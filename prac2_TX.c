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

char buffer[6];
char wholeNum[6];
char decimalNum[6];
unsigned int whole, decimal;
bit rb0_flag = 0;

void interrupt ISR()
{
    GIE = 0;

    if(INTF)
    {
        INTF = 0;
        rb0_flag = 1;
    }

    GIE = 1;
}

void initADC()
{
    ADCON1 = 0x80;
    ADCON0 = 0x41;
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

unsigned int readADC()
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

void instCtrl(unsigned char INST) 
{
    PORTD = INST; // load instruction to PORTD
    RB5 = 0;      // set RS to 0 (instruction register)
    RB6 = 0;      // set RW to 0 (write)
    RB7 = 1;      // set E to 1
    delayLCD(1);  // 1 ms delay (estimate)
    RB7 = 0;      // set E to 0 (strobe)
}

void dataCtrl(unsigned char DATA) 
{
    PORTD = DATA; // load data to PORTD
    RB5 = 1;      // set RS to 1 (data register)
    RB6 = 0;      // set RW to 0 (write)
    RB7 = 1;      // set E to 1
    delayLCD(1);  // 1 ms delay (estimate)
    RB7 = 0;      // set E to 0 (strobe)
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

void voltageReading()
{
    unsigned int adcValue = readADC();
    float voltage = (adcValue * (5.0 / 1024.0))+0.05; //assuming 5v reference voltage
    sprintf(buffer, "%.2f V", voltage); // Format voltage as string
    instCtrl(0x9D); // Move to new position on LCD
    displayString(buffer);

    whole = (unsigned int)voltage;                            
    decimal = (unsigned int)((voltage - whole) * 10.0);       
}

void initUSART_TX(void)
{
    TRISC6 = 0;
    TRISC7 = 1;

    SPBRG = 25;
    BRGH = 1;
    SYNC = 0;
    SPEN = 1;
    TXEN = 1;
}

void sendChar(int ch)
{
    while(!TRMT); //TXIF - if na transmit naba | TMRT idk
    TXREG = ch;
}

void main()
{
    // Port initialization
    TRISB = 0x0F;
    TRISD = 0x00;

    //Interrupt initialization
    INTEDG = 1;   // Interrupt at rising edge
    INTE = 1;     // Enable RB0/INT external interrupt
    INTF = 0;     // Clears the interrupt flag

    // ADIF = 0;
    // ADIE = 1;
    PEIE = 1;
    GIE = 1;

    initLCD();
    initADC();
    initUSART_TX();

    while(1)
    {
        instCtrl(0x80);
        displayString("Voltage: ");
        // instCtrl(0xC0);
        // displayString("TESTING: "); //MO WORK
        voltageReading();

        if(rb0_flag == 1)
        {
            
            rb0_flag = 0;
            sendChar(whole);
            sendChar(decimal);
            // while(rb0_flag);
        }
    }


}
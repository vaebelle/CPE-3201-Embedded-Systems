#include <xc.h> // include file for the XC8 compiler

#pragma config FOSC = XT
#pragma config WDTE = OFF
#pragma config PWRTE = ON
#pragma config BOREN = ON
#pragma config LVP = OFF
#pragma config CPD = OFF
#pragma config WRT = OFF
#pragma config CP = OFF

void main()
{
    SPBRG = 0x19; //9.6k baud rate @ FOSC=4MHz, asynchronous high speed

    SYNC = 0;
    SPEN = 1;
    TX9 = 0;
    BRGH = 1;
    TXEN = 1;

    for(;;)            //foreground routine
    {
        while(!TRMT);   //wait until transmit shift register is empty
        TXREG = 'A';    //write data to send to TXREG
    }
}
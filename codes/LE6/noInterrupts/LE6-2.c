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
    SPBRG = 0x19;

    SYNC = 0;
    SPEN = 1;
    RX9 = 0;
    BRGH = 1;
    CREN = 1;
    TRISB = 0x00;
    PORTB = 0x00;

    for(;;)
    {
        while(!RCIF);
        PORTB = RCREG;
    }
}
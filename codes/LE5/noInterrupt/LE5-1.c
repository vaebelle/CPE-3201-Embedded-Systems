#include <xc.h> // include file for the XC8 compiler

#pragma config FOSC = XT
#pragma config WDTE = OFF
#pragma config PWRTE = ON
#pragma config BOREN = ON
#pragma config LVP = OFF
#pragma config CPD = OFF
#pragma config WRT = OFF
#pragma config CP = OFF

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

int readADC (void)
{
    int temp = 0;
    delay(1000);
    GO = 1;

    while (GO_DONE == 1);

    temp = ADRESH;
    temp = temp << 8;
    temp = temp | ADRESL;
    return temp;
}

void main (void)
{
    int d_value = 0;
    
    TRISB = 0x00;   //set all PORTB as output
    PORTB = 0x00;   //all LEDs are off
    ADCON1 = 0x80;  //result: right Justified | clock: FOSC/2 | PORTA are analog | VREF+: VDD, VREF-: VSS 
    ADCON0 = 0x01;  //clock: FOSC/2 | analog channel: AN0 |  A/D conversion: STOP | A/D module: ON

    while(1)
    {
        d_value = readADC(); //get ADC value

        /*setting the LEDs*/
        if (d_value >= 0 && d_value <= 169)
        {
            PORTB = 0x00;
        }
        else if (d_value >= 170 && d_value <= 340)
        {
            PORTB = 0x01;
        }
        else if (d_value >= 341 && d_value <= 511 )
        {
            PORTB = 0x03;
        }
        else if (d_value >= 512 && d_value <= 682 )
        {
            PORTB = 0x07;
        }
        else if (d_value >= 683 && d_value <= 853 )
        {
            PORTB = 0x0F;
        }
        else if (d_value >= 854 && d_value <= 1024 )
        {
            PORTB = 0x1F;
        }
    }
}
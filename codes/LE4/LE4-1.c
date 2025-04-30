#include <xc.h> // include file for the XC8 compiler

#pragma config FOSC = XT
#pragma config WDTE = OFF
#pragma config PWRTE = ON
#pragma config BOREN = ON
#pragma config LVP = OFF
#pragma config CPD = OFF
#pragma config WRT = OFF
#pragma config CP = OFF

void interrupt ISR()
{
    GIE = 0;        //disable all unmasked interrupts (INTCON reg)

    if(TMR1IF == 1) //checks timer1 interrup flag
    {
        TMR1IF = 0; //clears interrupt flag
        TMR1 = 0x0BDC;  //timer will start couning at 0c0BDC

        RA0 = RA0^1;    //toggles the LED at RA0
    } 

    GIE = 1;        //enable all unmasked interrupts (INTCON reg)
}
void main()
{
    ADCON1 = 0x06;  //set all pins in PORTA as digital I/O
    TRISA = 0x00;   //set all of PORTA to output
    RA0 = 0;        //initialize RA0 to 0 (LED off)
    T1CON = 0x30;   //1:8 prescaler, internal clock, Timer1 off
    TMR1IE = 1;     //enable Timer1 overflow interrupt (PIE1 reg)
    PEIE = 1;       //enable all peripheral interrupt (INTCON reg)
    GIE = 1;        //enable all unmasked interrupts (INTCON reg)
    TMR1 = 0x0BDC;  //counter starts counting at 0x0BDC (3036)
    TMR1ON = 1;     //turns on timer 1

    while(1)        //foreground routine
    {
    }
}
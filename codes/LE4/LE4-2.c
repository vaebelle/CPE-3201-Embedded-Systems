/*
Author:     Bea Belle Therese B. Caños
Course:     CpE 3201
Activity:   LE4-2 | Timer2
*/

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
    GIE = 0;        //disable all unmasked interrupts

    if(TMR2IF == 1) //checks timer2 interrupt flag (TMR2 == PR2)
    {
        TMR2IF = 0; //clears interrupt flag
        RA0 = RA0^1;
    }

    GIE = 1;        //enable all unmasked interrupts
}

void main()
{
    ADCON1 = 0x06;  //set all pins in PORTA as digital I/O
    TRISA = 0x00;   //sets all pins of PORTA to output
    RA0 = 0;        //initialize RA0 to 0
    T2CON = 0x01;   //1:4 prescaler, Timer2 off
    TMR2IE = 1;     //enable timer2/PR2 match interrupt
    TMR2IF = 0;     //reset interrup flag (PIR1 reg)
    PEIE = 1;       //enable all peripheral interrupt
    GIE = 1;        //enable all unmasked interrupts
    PR2 = 0x7D;     //match value for TMR2 (125) a half cycle
    TMR2ON = 1;     //turns on timer2

    while(1)
    {

    }
}
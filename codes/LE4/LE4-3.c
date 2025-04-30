/*
Author:     Bea Belle Therese B. Caños
Course:     CpE 3201
Activity:   LE4-3 | Capture Module
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
    int period = 0;
    GIE = 0;        //disable all unmasked interrupts
    
    if(CCP1IF == 1) //checks CCP1 interrupt flag
    {
        CCP1IF = 0; //clears interrupt flag
        TMR1 = 0;   //resets tmr1

        period = CCPR1/1000;    //transfers captured TMR1 value

        period = period * 8;    //multiply by the normalized TMR1 timeout
    }

    GIE = 1;        //enable all unmasked interrupts
}

void main()
{
    TRISC = 0x04;   //set RC2 to input
    T1CON = 0x30;   //1:8 prescaler, Timer1 off
    CCP1CON = 0x05; //capture mode: every rising edge
    CCP1IE = 1;     //enable TMR1/CCP1 match interrupt (PIE1 reg)
    CCP1IF = 0;     //reset interrupt flag
    PEIE = 1;       //enable all peripheral interrupt
    GIE = 1;        //enable all unmasked interrupts
    TMR1ON = 1;     //turns on timer 1

    while(1)
    {

    }
}
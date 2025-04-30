/*
Author:     Bea Belle Therese B. Caños
Course:     CpE 3201
Activity:   LE4-4 | Compare Module
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
    GIE = 0;            //disable all unmasked interrupts

    if(CCP1IF == 1)     //checks CCP1 interrupt flag
    {
        CCP1IF = 0;     //clears interrupt flag
        RA0 = RA0^1;    //toggles the output signal at RA0
    }

    GIE = 1;            //enable all unmasked interrupts
}

void main()
{
    ADCON1 = 0x06;      //sets all pins in PORTA as digital I/O
    TRISA = 0x00;       //sets all of PORTA to output
    RA0 = 0;            //initialize RA0 to 0
    T1CON = 0x20;       //1:4 prescaler, Timer1 off
    CCP1CON = 0x0A;     //compare mode: generate interrupt on match
    CCP1IE = 1;         //enable TMR1/CCP1 match interrupt
    CCP1IF = 0;         //reset interrupt flag
    CCPR1 = 0x4E2;       //set the match value to TMR1

    /*
    solution to get 0x4E2: using a prescaler of 1:4 for timer1, we can calculate the timer max count
    at half-cycle:
    0.005s = (1)/(4MHz/4)*4*TimerMaxCount
    TimerMaxCount = 1250 or 0x4E2
    */

    PEIE = 1;           //enable all peripheral interrupt
    GIE = 1;            //enable all unmasked interrupt
    TMR1ON = 1;         //turns on timer1

    while(1)
    {

    }
}
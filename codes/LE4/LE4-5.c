/*
Author:     Bea Belle Therese B. Caños
Course:     CpE 3201
Activity:   LE4-5 | PWM
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

void main()
{
    /* following the steps in setting up PWM */
    PR2 = 0x7C;     //set value for PR2
    CCPR1L = 0x57;  //set value for (8MSBs)
    CCP1CON = 0x2C; //set value for (2LSBs), PWM mode
    TRISC = 0x00;   //sets all of PORTC (RC2) to output
    RC2 = 0;        //initialize RC2 to 0
    T2CON = 0x06;   //1:16 prescaler, timer2 on

    while(1)        //foreground routine
    {
    }
}
/*
Author:     Bea Belle Therese B. Caños
Course:     CpE 3201
Activity:   LE4-7 | Duty Cycles and Frequency
*/

#include <xc.h>

#pragma config FOSC = XT
#pragma config WDTE = OFF
#pragma config PWRTE = ON
#pragma config BOREN = ON
#pragma config LVP = OFF
#pragma config CPD = OFF
#pragma config WRT = OFF
#pragma config CP = OFF

unsigned int period_flag = 0;
float period = 0.001;
unsigned int duty_flag = 0;
float percent = 0;
unsigned int result = 0;
float time = 0;

void interrupt ISR()
{

	GIE = 0;
	if(TMR2IF){
		TMR2IF = 0;		
	}

	GIE = 1;
}

void main()
{	
	TRISB = 0x03;
	TRISC = 0x00;
	
	// set value for PR2  
	PR2 = 0x3E;        
	
	CCPR1L = 0x06;     
	CCP1CON = 0x1C;    
     
	RC2 = 0;           
	T2CON = 0x06; 

	while(1)
	{
		if(RB0){  //PWM Period
			period_flag++;
			if(period_flag == 0){ //1k Hz
				PR2 = 0x3E;
				period = 0.001;
			}
			else if(period_flag == 1){ //1.5k Hz
				PR2 = 0x29;
				period = 0.007;
			}
			else if(period_flag == 2){ //10k Hz
				PR2 = 0x05;
				period = 0.0001;
			}
			else if (period_flag > 2){
				period_flag = 0;
				period = 0.001;
				PR2 = 0x3E;
				
			}
			while(RB0){}
		}
		
		if(RB1){
			duty_flag++;
			if(duty_flag == 0){
				percent = 0.1;
			}
			else if(duty_flag == 1){
				percent = 0.25;	
			}
			else if(duty_flag == 2){
				percent = 0.5;	
			}
			else if(duty_flag == 3){
				percent = 0.75;	
			}
			else if(duty_flag == 4){
				percent = 0.95;	 
			}
			else if(duty_flag > 4){
				duty_flag = 0;
				percent = 0.1; 	
			}
			while(RB1){}
		}
		
		if(period == 0.001){
			if(percent == 0.1){
				CCPR1L = 0x06;
				CCP1CON =  0x1C;
			}
			else if(percent == 0.25){
				CCPR1L = 0x0F;
				CCP1CON = 0x3C;
			}
			else if(percent == 0.5){
				CCPR1L = 0x1F;
				CCP1CON = 0x1C;
			}
			else if(percent == 0.75){
				CCPR1L = 0x2F;
				CCP1CON = 0x0C;
			}
			else if(percent == 0.95){
				CCPR1L = 0x3B;
				CCP1CON = 0x2C;
			}
			else{
				CCPR1L = 0x06;
				CCP1CON =  0x1C;
			}
		}
		else if(period == 0.007){
			if(percent == 0.1){
				CCPR1L = 0x04;
				CCP1CON = 0x1C;
			}
			else if(percent == 0.25){
				CCPR1L = 0x0A;
				CCP1CON = 0x2C;
			}
			else if(percent == 0.5){
				CCPR1L = 0x14;
				CCP1CON = 0x3C;
			}
			else if(percent == 0.75){
				CCPR1L = 0x1F;
				CCP1CON = 0x1C;
			}
			else if(percent == 0.95){
				CCPR1L = 0x27;
				CCP1CON = 0x2C;
			}
			else{
				CCPR1L = 0x04;
				CCP1CON = 0x1C;
			}	
		}
		else if(period == 0.0001){
			if(percent == 0.1){
				CCPR1L = 0x00;
				CCP1CON =  0x3C;
			}
			else if(percent == 0.25){
				CCPR1L = 0x01;
				CCP1CON = 0x2C;
			}
			else if(percent == 0.5){
				CCPR1L = 0x03;
				CCP1CON = 0x1C;
			}
			else if(percent == 0.75){
				CCPR1L = 0x04;
				CCP1CON = 0x3C;
			}
			else if(percent == 0.95){
				CCPR1L = 0x06;
				CCP1CON = 0x0C;
			}
			else{
				CCPR1L = 0x00;
				CCP1CON =  0x3C;
			}
		}
		
	}
}

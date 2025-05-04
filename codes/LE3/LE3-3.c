/*
NAME: Bea Belle Therese B. Caños
INSTRUCTOR: Engr. Joseph Alvin Macapagal
COURSE: CpE 3201
*/

#include<xc.h> // include file for the XC8 compiler
#pragma config FOSC = XT
#pragma config WDTE = OFF
#pragma config PWRTE = ON
#pragma config BOREN = ON
#pragma config LVP = OFF
#pragma config CPD = OFF
#pragma config WRT = OFF
#pragma config CP = OFF

bit TMR0IF_flag = 0;
bit INTF_flag = 0;
bit Load_flag = 0;
int load = 0x00;

void interrupt ISR()
{
	int DATA;
	GIE = 0;

	if(INTF == 1){
		INTF = 0;
		INTF_flag = 1;
		DATA = PORTD & 0x0F;

		if(DATA == 0x00){
			Load_flag = 1;
			load = 0x01;
		}
		else if(DATA == 0x01){
			Load_flag = 1;
			load = 0x02;
		}
		else if(DATA == 0x02){
			Load_flag = 1;
			load = 0x03;
		}
		else if(DATA == 0x04){
			Load_flag = 1;
			load = 0x04;
		}
		else if(DATA == 0x05){
			Load_flag = 1;
			load = 0x05;
		}
		else if(DATA == 0x06){
			Load_flag = 1;
			load = 0x06;
		}
		else if(DATA == 0x08){
			Load_flag = 1;
			load = 0x07;
		}
		else if(DATA == 0x09){
			Load_flag = 1;
			load = 0x08;
		}
		else if(DATA == 0x0A){
			Load_flag = 1;
			load = 0x09;
		}


	}
	else if(TMR0IF == 1){
		TMR0IF = 0;
		TMR0IF_flag = 1;
	}


	GIE = 1;
}

void delay(int count)
{
	int OF_count = 0;
    
    while (OF_count < count)
    {
        if (TMR0IF_flag == 1)
        {
            OF_count++;
            TMR0IF_flag = 0;
        }
    }
}

void main()
{	
	unsigned char counter = 0x01;

	OPTION_REG = 0xC4;
	
	TRISB = 0xFF;
	TRISC = 0x00;
	TRISD = 0xFF;


	INTEDG = 1; // interrupt at rising edge
 	INTE = 1; // enable RB0/INT external interrupt
 	INTF = 0; // clears the interrupt flag

	TMR0IF = 0;
    TMR0IE = 1;

	GIE = 1; // enables all unmasked interrupt

	PORTC = 0x00;

	while(1)
	{
		if(INTF_flag == 0)
		{
			while(INTF_flag != 1){
			}
		}
		 else if(counter == 0x09)
        {
            counter = 0x00;
        }
		else if(Load_flag == 1){
		
			counter = load;
		}
        else
        {
            counter++;
        }
        
        Load_flag = 0;
        delay(98);
        PORTC  = counter;
	}
}
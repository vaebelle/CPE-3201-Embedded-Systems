#include<xc.h> // include file for the XC8 compiler
#pragma config FOSC = XT
#pragma config WDTE = OFF
#pragma config PWRTE = ON
#pragma config BOREN = ON
#pragma config LVP = OFF
#pragma config CPD = OFF
#pragma config WRT = OFF
#pragma config CP = OFF

unsigned int adc_result = 0;
unsigned int whole, decimal;
bit control_signal = 0;

void delay(int MUL) {
    int i, j;
    for (i = MUL; i != 0; i--) {
        for (j = 0; j < 1; j++) {} 
    }
}

void interrupt ISR()
{
	int d_value = 0;
	GIE = 0;
	if(ADIF){
		ADIF = 0;
		
		adc_result = (ADRESH << 8) | ADRESL;
		
		float voltage = ((adc_result * 5.0) / 1024);
        unsigned int display_value = (unsigned int)(voltage * 10); 


        whole = display_value / 10; 
        decimal = display_value % 10; 	
	}

	GIE = 1;
	delay(10);
	GO = 1;
}


void main()
{	
	int d_value = 0;
	TRISC = 0x00;
	TRISB = 0x00;
	TRISA = 0xFF;
	PORTB = 0x00;
	ADCON1 = 0x80;		//result: right justified, clock: FOSC/8
						//all ports in PORTA are analog
						//VREF+ = VDD, VREF- = VSS
						
	ADCON0 = 0x41;		// clock: FOSC/8, analog channel:AN0(RA0)
						//A/D Conversion: STOP, A/D Module: ON
	ADIF = 0;
	ADIE = 1;
	PEIE = 1;
	GIE = 1;
	delay(10);
	GO = 1;
	PORTB = 0x00;
	while(1)
	{
		
        if(control_signal==0){
        	PORTB = whole & 0x0F;
        	RC2 = 1; 				// decimal point
        	control_signal = 1;
        	RC0 = 0; 				// whole number
        	RC1 = 1;				// decimal		
        }
        else if(control_signal == 1){
	        	PORTB = decimal & 0x0F;
	        	RC2 = 0;			// decimal point
	        	control_signal = 0;
	        	RC0 = 1;			// whole number
	        	RC1 = 0;			// decimal    	
       	} 
	}	
		
}
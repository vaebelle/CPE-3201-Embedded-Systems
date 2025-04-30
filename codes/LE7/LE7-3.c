#include<xc.h> // include file for the XC8 compiler
#pragma config FOSC = XT
#pragma config WDTE = OFF
#pragma config PWRTE = ON
#pragma config BOREN = ON
#pragma config LVP = OFF
#pragma config CPD = OFF
#pragma config WRT = OFF
#pragma config CP = OFF

unsigned int RH = 0, T = 0;
unsigned int RH_tens, RH_ones, Temp_tens, Temp_ones;

void delayLCD(int MUL) {
    int i, j;
    for (i = MUL; i != 0; i--) {
        for (j = 0; j < 256; j++) {} 
    }
}

void instCtrl (unsigned char INST)
{
	PORTB = INST; // load instruction to PORTB
	RD0 = 0; // set RS to 0 (instruction reg)
	RD1 = 0; // set RW to 0 (write)
	RD2 = 1; // set E to 1
	delayLCD(1); // 1 ms delay (estimate)
	RD2 = 0; // set E to 0 (strobe)
}

void dataCtrl (unsigned char DATA)
{
	PORTB = DATA; 		// load data to PORTB
	RD0 = 1; 			// set RS to 1 (data reg)
	RD1 = 0; 			// set RW to 0 (write)
	RD2 = 1; 			// set E to 1
	delayLCD(1); 			// 1 ms delay (estimate)
	RD2 = 0; 			// set E to 0 (strobe)
}

void initLCD() {
    delayLCD(15);
    instCtrl(0x38); // 8-bit mode, 2-line display
    instCtrl(0x0C); // Display ON, Cursor OFF
    instCtrl(0x06); // Entry mode
    instCtrl(0x01); // Clear display
    delayLCD(2);
}

void display(const char *input) {
    int i;
    for (i = 0; input[i] != '\0'; i++) {
        dataCtrl(input[i]);
    }
}

void init_I2C_Master(){
 TRISC3 = 1; 		// set RC3 (SCL) to input
 TRISC4 = 1; 		// set RC4 (SDA) to input
 SSPCON = 0x28; 	// SSP enabled, I2C master mode
 SSPCON2 = 0x00; 	// start condition idle, stop condition idle
 					// receive idle
 SSPSTAT = 0x00; 	// slew rate enabled
 SSPADD = 0x09; 	// clock frequency at 100 KHz (FOSC = 4MHz) 
					//Formula SSPADD = (Fosc/(4 * CLK)) - 1
}

void I2C_Wait(void)
{
 	/* wait until all I2C operation are finished*/
 	while((SSPCON2 & 0x1F) || (SSPSTAT & 0x04));
} 

void I2C_Start(void)
{
	 /* wait until all I2C operation are finished*/
	 I2C_Wait();
	 /* enable start condition */
	 SEN = 1; // SSPCON2
}
void I2C_Stop(void)
{
	 /* wait until all I2C operation are finished*/
	 I2C_Wait();
	 /* enable stop condition */
	 PEN = 1; // SSPCON2
} 

void I2C_RepeatedStart(void)
{
	 /* wait until all I2C operation are finished*/
	 I2C_Wait();
	 /* enable repeated start */
	 RSEN = 1; // SSPCON2
}

void I2C_Send(unsigned char data)
{
	/*wait until all I2C operation are finished*/
	 I2C_Wait();
	 /* write data to buffer and transmit */
	 SSPBUF = data; 
}

unsigned char I2C_Receive(unsigned char ack)
{
	 unsigned char temp;
	 I2C_Wait(); 			// wait until all I2C operation are finished
	 RCEN = 1; 				// enable receive (SSPCON2 reg)
	 
	 I2C_Wait(); 			// wait until all I2C operation are finished
	 temp = SSPBUF; 		// read SSP buffer
	 
	 I2C_Wait(); 			// wait until all I2C operation are finished
	 ACKDT = (ack)?0:1; 	// set acknowledge (ACK) or not acknowledge (NACK)
	 ACKEN = 1; 			// enable acknowledge sequence
	 
	 return temp;
}

void delay(int MUL) {
    int i, j;
    for (i = MUL; i != 0; i--) {
        for (j = 0; j < 256; j++) {} 
    }
}

void displayHumidity(){
	instCtrl(0x80);
	display("Humidity: ");
	dataCtrl(RH_tens + '0');
	dataCtrl(RH_ones + '0');
	display(".0%");
}

void displayTemp(){
	instCtrl(0xC0);
	display("Temperature: ");
	dataCtrl(Temp_tens + '0');
	dataCtrl(Temp_ones + '0');
	display(".0 C");
}

/* Master Device*/
void main(void)
{
	 TRISB = 0x00;
	 TRISD = 0x00;
	 
	 unsigned int RH_temp = 0x0000, T_temp = 0x0000;
	 unsigned char temp = 0x00;
	 
	 init_I2C_Master();
	 initLCD();
	 while(1)
	 {
		 I2C_Start();
		 I2C_Send(0x80);
		 I2C_Send(0xE5);
		 I2C_RepeatedStart();
		 
		 I2C_Send(0x81);
		 RH_temp = I2C_Receive(1);
		 temp = I2C_Receive(0);
		 I2C_Stop;
		 delay(1);
		 
		 RH_temp = RH_temp << 6;
		 temp = temp >> 2;
		 RH_temp = RH_temp | temp;
		 
		 //Formula:
		 float rh = ((125*((float)RH_temp/16384)));
		 if(rh >= 6){
		 	rh = rh - 6;
		 }
		 else{
		 	rh = 0.00;
		 }
		 
		 RH = (unsigned int)rh;
		 
		 RH_tens = RH / 10;
		 RH_ones = RH % 10;
		 
		 displayHumidity();
		 
		 I2C_Start();
		 I2C_Send(0x80);
		 I2C_Send(0xE3);
		 I2C_Stop();
		 delay(1);
		 
		 temp = 0x00;
		 I2C_Start();
		 I2C_Send(0x80); 
		 I2C_Send(0xE3);
		 I2C_RepeatedStart();
		 I2C_Send(0x81);
		 
		 T_temp = I2C_Receive(1); 
		 temp = I2C_Receive(0); 
		 I2C_Stop(); 
		 delay(10); 

		 //T_temp = (T_temp << 8) | temp;
		 //float t = (175.72)*((float)T_temp / 65536);
		 
		 T_temp = T_temp << 6; 
		 temp = temp >> 2; 
		 T_temp = T_temp | temp;
		 
		 float t = (175.72)*((float)T_temp / 16384);
		 if(t >= 46.85){
		 	t = t - 46.85;
		 }
		 else{
		 	t = 0.00;
		 }
		 
		 T = (unsigned int)t;
		 
		 Temp_tens = T / 10;
		 Temp_ones = T % 10;
		 
		I2C_Start();    // initiate start condition
        I2C_Send(0x80); // send the slave address + write
        I2C_Send(0xFE); // SHT21, soft reset
        I2C_Stop();     // initiate stop condition
		delay(10);
		 
		 displayTemp();
	 }
} 
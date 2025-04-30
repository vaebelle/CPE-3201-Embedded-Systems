/*
Author:     Bea Belle Therese B. Caños
Course:     CpE 3201
Activity:   LE7-1| Master Mode
*/

#include <xc.h>

// Configuration bits
#pragma config FOSC = XT
#pragma config WDTE = OFF
#pragma config PWRTE = ON
#pragma config BOREN = ON
#pragma config LVP = OFF
#pragma config CPD = OFF
#pragma config WRT = OFF
#pragma config CP = OFF

/*INITIALIZATION*/
void init_I2C_Master()
{
    TRISC3 = 1;     //set RC3 (SCL) to input
    TRISC4 = 1;     //set RC4 (SDA) to input
    SSPCON = 0x28;  //SSP enabled, I2C master mode
    SSPCON2 = 0x00; //start condition idle, stop condition idle
                    //receive idle
    SSPSTAT = 0x00; //slew rate enabled
    SSPADD = 0x09;  //clock frequency at 100KHz (Fosc = 4MHz)
}

void I2C_Wait()
{
    /*wait auntil all I2C operation are finished*/
    while((SSPCON2 & 0x1F) || (SSPSTAT & 0x04));
}

void I2C_Start()
{
    /*wait untill all I2C operation are finished*/
    I2C_Wait();

    /*enable start condition*/
    SEN = 1;        //SSPCON2
}

void I2C_Stop()
{
    /*wait untill all I2C operation are finished*/
    I2C_Wait();

    /* enable stop condition */ 
    PEN = 1;    // SSPCON2 
}

void I2C_RepeatedStart()
{
    /*wait untill all I2C operation are finished*/
    I2C_Wait();

    /* enable stop condition */ 
    RSEN = 1;    // SSPCON2 
}

void I2C_Send(unsigned char data)
{
    /*wait untill all I2C operation are finished*/
    I2C_Wait();

    /* enable stop condition */ 
    SSPBUF = data;    // SSPCON2 
}

unsigned char I2C_Receive(unsigned char ack)
{
    unsigned char temp;

    I2C_Wait();     //wait until all I2C operation are finished
    RCEN = 1;       //enable receive

    I2C_Wait();     //wait until all I2C operation are finished
    temp = SSPBUF;  //read SSP buffer

    I2C_Wait();     //wait until all I2C operation are finished
    ACKDT = (ack) ? 0:1;        //set acknowledge or not acknowledge
    ACKEN = 1;      //enable acknowledge sequence

    return temp;
}

void delay(int MUL) {
    int i, j;
    for (i = MUL; i != 0; i--) {
        for (j = 0; j < 256; j++) {} 
    }
}
void main()
{
    TRISD = 0xFF;   //set all bits in PORTD as inputs
    init_I2C_Master;    //initialize I2C as master

    while(1)
    {
        I2C_Start();
        I2C_Send(0x10); //send the slave address + write
        I2C_Send(PORTD);//send 8-bit data frame
        I2C_Stop();     //initiate stop condition
        delay(200);
    }
}
#include <xc.h> // include file for the XC8 compiler
#include <stdio.h>

#pragma config FOSC = XT
#pragma config WDTE = OFF
#pragma config PWRTE = ON
#pragma config BOREN = ON
#pragma config LVP = OFF
#pragma config CPD = OFF
#pragma config WRT = OFF
#pragma config CP = OFF

#define _XTAL_FREQ 4000000

void instCtrl(unsigned char INST)
{
  PORTD = INST;
  RB5 = 0; // RS = 0 to accept data
  RB6 = 0;
  RB7 = 1; // E = 1 (write)
  __delay_ms(1);
  RB7 = 0; // E = 0 (strobe)
}

void dataCtrl(unsigned char DATA)
{
  PORTD = DATA;
  RB5 = 1; // RS = 1 for data control
  RB6 = 0;
  RB7 = 1; // E = 1 (write)
  __delay_ms(1);
  RB7 = 0; // E = 0 (strobe)
}

void initLCD()
{
  __delay_ms(1);
  instCtrl(0x38); // function set: 8-bit; dual-line
  instCtrl(0x08); // display off
  instCtrl(0x01); // display clear
  instCtrl(0x06); // entry mode: increment; shift off
  instCtrl(0x0C); // display on; cursor off; blink off
}

void displayString(const char *str)
{
  while (*str)
  {
    dataCtrl(*str);
    str++;
  }
}

void init_I2C_Master(void)
{
  TRISC3 = 1;     // set RC3 (SCL) to input
  TRISC4 = 1;     // set RC4 (SDA) to input
  SSPCON = 0x28;  // SSP enabled, I2C master mode
  SSPCON2 = 0x00; // start condition idle, stop condition idle
                  // receive idle
  SSPSTAT = 0x00; // slew rate enabled
  SSPADD = 0x09;  // clock frequency at 100 KHz (FOSC = 4MHz)
}

void I2C_Wait(void)
{
  /* wait until all I2C operation are finished*/
  while ((SSPCON2 & 0x1F) || (SSPSTAT & 0x04))
    ;
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
  /* wait until all I2C operation are finished*/
  I2C_Wait();
  /* write data to buffer and transmit */
  SSPBUF = data;
}

unsigned char I2C_Receive(unsigned char ack)
{
  unsigned char temp;
  I2C_Wait();            // wait until all I2C operation are finished
  RCEN = 1;              // enable receive (SSPCON2 reg)
  I2C_Wait();            // wait until all I2C operation are finished
  temp = SSPBUF;         // read SSP buffer
  I2C_Wait();            // wait until all I2C operation are finished
  ACKDT = (ack) ? 0 : 1; // set acknowledge (ACK) or not acknowledge (NACK)
  ACKEN = 1;             // enable acknowledge sequence
  return temp;
}

void main(void)
{
  TRISB = 0x00;
  TRISD = 0x00;

  initLCD();
  instCtrl(0x80);
  displayString("Humidity:");
  instCtrl(0xC0);
  displayString("Temperature:");

  init_I2C_Master(); // initialize I2C as master

  while (1)
  {
    unsigned int MSBH = 0, LSBH = 0, MSBT = 0, LSBT = 0;
    float humidity = 0.0, temperature = 0.0;
    char humidityStr[4], temperatureStr[4];

    // Read and Write Humidity
    I2C_Start();    // initiate start condition
    I2C_Send(0x80); // send the slave address + write
    I2C_Send(0xE5); // SHT21, read humidity, hold master
    I2C_RepeatedStart();
    I2C_Send(0x81);
    MSBH = I2C_Receive(1); // read MSB of humidity data and acknowledge
    LSBH = I2C_Receive(0); // read LSB of humidity data and acknowledge
    I2C_Stop();            // initiate stop condition
    __delay_ms(50);        // delay before next operation

    LSBH &= 0xFC;
    MSBH <<= 8;
    MSBH |= LSBH;

    humidity = -6.0 + (125.0 * (MSBH / 65536.0));
    sprintf(humidityStr, "%f", humidity);

    instCtrl(0x8A);
    // dataCtrl(0x31);
    // dataCtrl(0x30);
    dataCtrl(humidityStr[0]);
    dataCtrl(humidityStr[1]);
    dataCtrl('%');

    // Read and Write Temperature
    I2C_Start();    // initiate start condition
    I2C_Send(0x80); // send the slave address + write
    I2C_Send(0xFE); // SHT21, soft reset
    I2C_Send(0xE3); // SHT21, read temperature, hold master
    I2C_RepeatedStart();
    I2C_Send(0x81);
    MSBT = I2C_Receive(1); // read MSB of temperature data and acknowledge
    LSBT = I2C_Receive(0); // read LSB of temperature data and acknowledge
    I2C_Stop();            // initiate stop condition
    __delay_ms(50);        // delay before next operation

    LSBT &= 0xFC;
    MSBT <<= 8;
    MSBT |= LSBT;

    temperature = -46.85 + (175.72 * (MSBT / 65536.0));
    sprintf(temperatureStr, "%f", temperature);

    instCtrl(0xCD);
    dataCtrl(temperatureStr[0]);
    dataCtrl(temperatureStr[1]);
    dataCtrl(temperatureStr[2]);
    dataCtrl(temperatureStr[3]);
    dataCtrl(0xDF);
    dataCtrl("C");

    I2C_Start();    // initiate start condition
    I2C_Send(0x80); // send the slave address + write
    I2C_Send(0xFE); // SHT21, soft reset
    I2C_Stop();     // initiate stop condition
    __delay_ms(50); // delay before next operation
  }
}
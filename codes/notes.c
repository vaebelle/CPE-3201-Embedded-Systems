 /*
    RB0 = interrupt pin

    interrup configuration:
    # = interrupt ra from rb0
    * = use of timer0

    OPTION_REG
        -(RBPU)' [portb pull-up enable bit]
            1 = disbable
            0 = enabled
        #*-INTEDG [interrupt edge select bit]
            1 = rising edge
            0 = falling edge
        *-T0CS [tmr0 clock source select bit]
            1 = transiion on RA4/0CKI pin
            0 = intercation intruction cycle clock
        *-TOSE [tmr0 source edge select bit]
            1 = incremen on high-to-low transion on RA4/T0CKL pin
            0 = increment on low-to-high transtion on RA4/T0CKL pin
        *-PSA [prescalar assignment bit]
            1 = prescalar assigned to WDT
            0 = prescalar assigned to Timer0 module
        *-PS2=PS0 [prescalar rate select bits]
    
    INTCON
        #*-GIE [global interrupt enable bit]
            1 = enable all unmasked bits
            0 = disable all interrupts

        -PEIE [peripheral interrupt enable bit]
            1 = enables all unmased peripheral interrupts
            0 = disables all periphral interrupts

        *-TMR0IE [tmr0 oferflow interrupt enable bit]
            1 = enables timer0 interrupt
            0 = disables timer0 interrupt

        #-INTE [rb0/int external interrupt enable bit]
            1 = enables rb0/int external interrupt
            0 = disables rbo/int external interrupt

        -RBIE [rb port change interrupt enable bit]
            1 = enables the rb port change interrupt
            0 = disables the rb port change inerrupt

        *-TMR0IF [tmro overflow interrup flag bit]
            1 = tmr0 register has overflowed (must be cleared in software)
            0 = tmr0 did not overflow

        #-INTF [rbo/int external interrupt flag bit]
            1 = rb0/int external interrupt occured (must be cleared in software)
            0 = no external interrupt occured

        -RBIF [rb port change interrupt flag bit]

    */


    /*
    CONFIGURATION FUNCTIONS:

    bit TMR0IF_flag = 0;
bit INTF_flag = 0;

void interrupt ISR()
{
    GIE = 0;       // disables all unmasked interrupts to prevent overlap
    if (INTF == 1) // check the interrupt flag
    {
        INTF = 0;
        INTF_flag ^= 1;
    }
    else if (TMR0IF == 1)
    {
        TMR0IF = 0;
        TMR0IF_flag = 1;
    }
    GIE = 1;
}

// timer0 delay function
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

// delay for lCDfunction:
void delayLCD(int MUL)
{
    int i, j;
    for (i = MUL; i != 0; i--)
    {
        for (j = 0; j < 256; j++)
        {
        }
    } // loop until i = 0
}

void instCtrl(unsigned char INST)
{
    PORTC = INST; // load instruction to PORTB
    RE0 = 0;      // set RS to 0 (instruction reg)
    RE1 = 0;      // set RW to 0 (write)
    RE2 = 1;      // set E to 1
    delayLCD(1);  // 1 ms delay (estimate)
    RE2 = 0;      // set E to 0 (strobe)
}

void dataCtrl(unsigned char DATA)
{
    PORTC = DATA; // load data to PORTB
    RE0 = 1;      // set RS to 1 (data reg)
    RE1 = 0;      // set RW to 0 (write)
    RE2 = 1;      // set E to 1
    delayLCD(1);  // 1 ms delay (estimate)
    RE2 = 0;      // set E to 0 (strobe)
}

void initLCD()
{
    delayLCD(1);    // LCD startup about approx 15ms
    instCtrl(0x38); // function set: 8-bit; dual-line
    instCtrl(0x08); // display off
    instCtrl(0x01); // display clear
    instCtrl(0x06); // entry mode: increment; shift off
    instCtrl(0x0E); // display on; cursor on; blink off
}


    
    */

#include "Sensors.h"

uint8 contended;

uint8 toggleCount;

void toggle()
{
    LATBINV = 1<<8;
    toggleCount++;
    if (toggleCount >= 22)
    {
        disable_Timer(SCK_RST);
        enable_Timer(RESET_TIMER);
        I2C1CONbits.ON = 1;
        TMR3 = TMR4 = 0;
        contended = 0;
        IFS1bits.I2C1MIF = 1; //Interrupt I2C to ensure that it runs
        LATA &= ~(1<<3);
    }
}

void reset()
{
    LATA |= 1<<3;
    //If this is called, we must send 9 clocks on the I2C clock line to clear bus contention
    I2C1CONbits.ON = 0;
    contended = 1;
    toggleCount = 0;
    enable_Timer(SCK_RST);
    disable_Timer(RESET_TIMER);
    LATBbits.LATB8 = 1; //output a 1
    TRISBbits.TRISB8 = 0;
}

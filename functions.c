#include "functions.h"

unsigned int read_buttons(void)
{
    unsigned int buttons = 0;

    buttons = PORTRead(IOPORT_A) & (BIT_6 | BIT_7);

    buttons = buttons << 4;
}


void write_leds(unsigned int current_state)
{
    PORTWrite(IOPORT_B, current_state);
}

void setup_i2c (I2C_MODULE i2c_id)
{

    I2CSetFrequency(i2c_id, SYS_CLK_FREQ, I2C_CLK_FREQ);
    I2CEnable(i2c_id, TRUE);

    return;
}

void i2c_1_isr(void)
{
    
    
    
    i2c_status = I2CGetStatus(I2C1);

    if ((i2c_status & I2C_START) == I2C_START)
    {
        //start condition was just sent, add code for sending

    }
    else if ((i2c_status & I2C_START) == I2C_START)
    {

    }

}


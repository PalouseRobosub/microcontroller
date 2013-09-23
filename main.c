 /********************************************************
 *   File Name: main.c
 *
 *   Description:
 *              Main file
 *
 *
 *********************************************************/


#include "functions.h"

 int i2c_isr_state = 0;

int main(void)
{
    uint current_state = 0;
    
    i2c_isr_state = 0;
    //setup buttons
    PORTSetPinsDigitalIn(IOPORT_A, BIT_6 | BIT_7);
    
    //setup LEDs
    PORTSetPinsDigitalOut(IOPORT_B, BIT_10 | BIT_11);

    setup_i2c(I2C1);


    while (1)
    {
        current_state = read_buttons();

        write_leds(current_state);
    }

    return 0;
}





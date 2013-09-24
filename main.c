 /********************************************************
 *   File Name: main.c
 *
 *   Description:
 *              Main file
 *
 *
 *********************************************************/


#include "functions.h"
#include "Testbench.h"

 I2C_STATE i2c_state;
 uint8 QueueStart = 0;
 uint8 QueueEnd = 0;
 uint8 QueueLength = 0;
 Node I2CQueue[QueueSize];

int main(void)
{
    queueTest();

    
    i2c_state = SENDING_START;
    //setup buttons
    PORTSetPinsDigitalIn(IOPORT_A, BIT_6 | BIT_7);
    
    //setup LEDs
    PORTSetPinsDigitalOut(IOPORT_B, BIT_10 | BIT_11);

    setup_i2c(I2C1);


    while (1)
    {


        i2c_1_isr();
//        current_state = read_buttons();
//
//        write_leds(current_state);
    }

    return 0;
}





/* 
 * File:   main.c
 * Author: James
 *
 * Created on September 21, 2013, 7:03 PM
 */

//#include <stdio.h>
//#include <stdlib.h>



#include "functions.h"



int main(void)
{
    unsigned int current_state = 0;
    
    //setup buttons
    PORTSetPinsDigitalIn(IOPORT_A, BIT_6 | BIT_7);
    
    //setup LEDs
    PORTSetPinsDigitalOut(IOPORT_B, BIT_10 | BIT_11);


    while (1)
    {
        current_state = read_buttons();

        write_leds(current_state);
    }

    return 0;
}





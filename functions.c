/********************************************************
 *   File Name: functions.c
 *
 *   Description:
 *              Main source code file
 *
 *
 *********************************************************/

#include "functions.h"

 /********************************************************
 *   Function Name:
 *
 *   Description:
 *
 *
 *********************************************************/
unsigned int read_buttons(void)
{
    unsigned int buttons = 0;

    buttons = PORTRead(IOPORT_A) & (BIT_6 | BIT_7);

    buttons = buttons << 4;
}

 /********************************************************
 *   Function Name:
 *
 *   Description:
 *
 *
 *********************************************************/
void write_leds(unsigned int current_state)
{
    PORTWrite(IOPORT_B, current_state);
}

 





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
 *   Function Name: read_buttons()
 *
 *   Description: Reads buttons from board
 *
 *
 *********************************************************/
unsigned int read_buttons(void)
{
    unsigned int buttons = 0;

    buttons = PORTRead(IOPORT_A) & (BIT_6 | BIT_7);

    buttons = buttons << 4;
}

void setup_leds(void)
{
    TRISBCLR = (0x0F << 10);
    PORTBCLR = (0x0F << 10);

}

 /********************************************************
 *   Function Name: write_leds(unsigned int)
 *
 *   Description: writes to LED's on board
 *
 *
 *********************************************************/
void write_leds(uint8 value)
{
    value = value & 0x0F; //mask to only 4 bits
    PORTBCLR = (0x0F << 10);
    PORTBSET = (value << 10);
}

void delay(void)
{
    uint i;
    for (i=0; i<10000; ++i);
    return;
}

void con_led(uint8 led, boolean on)
{
    if (on == TRUE)
    {
        PORTBSET = ((1 << 10) << led);
    }
    else
    {
        PORTBCLR = ((1 << 10) << led);
    }
}





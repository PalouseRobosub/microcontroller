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


    TRISAbits.TRISA0 = 0;
    TRISAbits.TRISA1 = 0;
    TRISAbits.TRISA5 = 0;
    TRISDbits.TRISD9 = 0;
    TRISDbits.TRISD13 = 0;
    TRISDbits.TRISD7 = 0;
    TRISGbits.TRISG0 = 0;
    TRISAbits.TRISA7 = 0;
    TRISGbits.TRISG12 = 0;
    TRISAbits.TRISA9 = 0;

    
#define init 0


    PORTAbits.RA0 = init;//bad
    PORTAbits.RA1 = init;//bad
    PORTAbits.RA5 = init;//bad
    PORTDbits.RD9 = init; //timer
    PORTDbits.RD13 = init; //i2c
    PORTDbits.RD7 = init; //comm_uart
    PORTGbits.RG0 = init; //motor_uart
    PORTAbits.RA7 = init;//bad
    PORTGbits.RG12 = init; //
    PORTAbits.RA9 = init;  //comm uart has sync

    

    



    /*
    70 RA0
    71 RA1
    73 RA5
    74 RD9
    76 RD13
    77 RD7
    78 RG1
    79
    81 RG0
    83 RG12
    85 RA9
    */

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





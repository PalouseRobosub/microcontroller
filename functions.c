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

#include "led.h"

extern uint8 led_buf[NUMLEDS*3 + (NUMLEDS-1)/32 + 1] = {0};
extern PIXEL led_frame_buf[12][NUMLEDS] = {0};
extern PIXEL *pixels = (PIXEL *) led_buf;


//timer subroutine
void Pattern_Select(uint8 *msg)
{
    Timer_Config pattern_timer;
    
    uint8  pattern = msg[0];
    uint16 *period = &(msg+1);

    pattern_timer.which_timer = Timer_2;
    pattern_timer.divide      = Div_256;
    pattern_timer.period      = *period * 58.6;
    pattern_timer.enabled     = 1;

    switch (pattern)
    {
        case reset_period:
            PR2 = *period * 58.6;
        case forward_warp_shift:
            forward_warp_shift ();
            break;
        case backward_warp_shift:
            backward_warp_shift ();
            break;
        case clockwise_shift:
            clockwise_shift ();
            break;
        case counterclockwise_shift:
            counterclockwise_shift ();
            break;
        case toggle:
            toggle ();
            break;
        default:
            break;
    }
    if(pattern_timer.period == 0)
        initialize_timer(pattern_timer);
}
void forward_warp_shift ()
{
    PIXEL tempL, tempR;
    uint8 i = 0;
    tempL = pixels [7];
    tempR = pixels [8];
    for (i=0; i <= 7; i++)
    {
        pixels [i+1] = pixels [i];
    }
    for (i=15; i >=8; i--)
    {
        pixels [i-1] = pixels [i];
    }
    pixels [0] = tempL;
    pixels [15] = tempR;
    send_SPI(SPI_CH_1, led_buf, sizeof(led_buf));
}

void backward_warp_shift ()
{
    PIXEL tempL, tempR;
    uint8 i = 7;
    tempL = pixels [0];
    tempR = pixels [15];
    for (i = 7; i >=1; i-- )
    {
        pixels [i-1] = pixels[i];
    }
    for (i = 8; i <= 14; i++)
    {
        pixels [i+1] = pixels[i];
    }
    pixels [7] = tempL;
    pixels [15] = tempR;
    send_SPI(SPI_CH_1, led_buf, sizeof(led_buf));
}

void clockwise_shift ()
{
    uint8 i = 0;
    PIXEL temp;
    temp = pixels [15];
    for (i =0; <= 14; i++)
    {
        pixels [i+1] = pixels [i];
    }
    pixels [0] = temp;
    send_SPI(SPI_CH_1, led_buf, sizeof(led_buf));
}

void counterclockwise_shift ()
{
    uint8 i = 0;
    PIXEL temp;
    temp = pixels [0];
    for (i = 15; i >= 1; i--)
    {
        pixels [i-1] = pixels [i];
    }
    pixels [15] = temp;
    send_SPI(SPI_CH_1, led_buf, sizeof(led_buf));
}
void toggle ()
{
    PIXEL *temp_ptr = 0;
    temp_ptr = pixels;
    pixels = toggle_pixels;
    toggle_pixels = temp_ptr;
}
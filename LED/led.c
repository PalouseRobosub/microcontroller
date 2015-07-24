#include "led.h"

//LED frame buffer
uint8 led_buf[NUMLEDS*3 + (NUMLEDS-1)/32 + 1] = {0};
PIXEL led_frame_buf[12][NUMLEDS] = {0};
PIXEL *pixels = (PIXEL *) led_buf;

void parse_packet(uint8 *buffer, uint8 size)
{
    switch(*buffer)
    {
        case manual:
            copy_strip(led_frame_buf[11] + (STRIPSIZE*buffer[1]), (PIXEL*) (buffer+2));
            break;
        case all:
            set_all(led_frame_buf[11], (PIXEL*) (buffer+1));
            break;
        case strip:
            set_strip(led_frame_buf[11] + (STRIPSIZE*buffer[1]), (PIXEL*) (buffer+2));
            break;
        case dual_all:
            set_dual_all(led_frame_buf[11], (PIXEL*) (buffer+1), (PIXEL*) (buffer+4));
            break;
        case dual_strip:
            set_dual_strip(led_frame_buf[11] + (STRIPSIZE*buffer[1]), (PIXEL*) (buffer+1), (PIXEL*) (buffer+4));
            break;
        case set:
            T2CONbits.ON = 0;
            memcpy(led_buf, led_frame_buf[buffer[0]]);
            send_SPI(SPI_CH_1, led_buf, sizeof(led_buf));
            break;
        case save_pattern:
            break;
        case start_pattern:
            break;
        default:
            break;
    }
}

void initialize_pins() {
    ANSELBbits.ANSB13 = 0;
    ANSELAbits.ANSA1 = 0;
    ANSELBbits.ANSB14 = 0;
    RPA1R = 0b0011; //set the RPA2R to SDO1
    U1RXR = 0b0011; // Set the pin to RPB13
}

void init_Timer (Timer_Config timer_config, uint16 period)
{
    timer_config.enabled = TRUE;
    timer_config .which_timer = 1;
    timer_config.divide = Div_256;
    timer_config.callback = timer_tick;
    timer_config.period = period;
    initialize_timer (timer_config);
}


void copy_strip(PIXEL *pixels, PIXEL *colors)
{
    memcpy(pixels, colors, STRIPSIZE*3);
}
void set_all(PIXEL *pixels, PIXEL *color)
{
    int i = 0;
    for(i = 0; i < NUMLEDS; i++)
    {
        pixels[i] = *color;
    }
}
void set_strip(PIXEL *pixels, PIXEL *color)
{
    int i = 0;
    for(i = 0; i < STRIPSIZE; i++)
    {
        pixels[i] = *color;
    }
}
void set_dual_all(PIXEL *pixels, PIXEL *color1, PIXEL *color2)
{
    int i = 0;
    for(i = 0; i < NUMLEDS; i++)
    {
        if(i&1)
        {
            pixels[i] = *color2;
        }
        else
        {
            pixels[i] = *color1;
        }
    }
}

void set_dual_strip(PIXEL *pixels, PIXEL *color1, PIXEL *color2)
{
    int i = 0;
    for(i = 0; i < STRIPSIZE; i++)
    {
        if(i&1)
        {
            pixels[i] = *color2;
        }
        else
        {
            pixels[i] = *color1;
        }
    }
}

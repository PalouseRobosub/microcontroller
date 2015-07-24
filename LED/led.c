#include "led.h"

//Timer mode
TimerMode mode = toggle;

//LED frame buffer
static uint8 led_buf[NUMLEDS*3 + (NUMLEDS-1)/32 + 1] = {0};
PIXEL *pixels = (PIXEL *) led_buf;

static uint8  led_toggle_buf[NUMLEDS*3 + (NUMLEDS-1)/32 + 1] = {0};
PIXEL *toggle_pixels = (PIXEL *) led_toggle_buf;

void parse_packet(uint8 *buffer, uint8 size)
{
    switch(*buffer)
    {
        case manual:
            copy_strip(pixels + (STRIPSIZE*buffer[1]), (PIXEL*) (buffer+2));
            break;
        case all:
            set_all(pixels, (PIXEL*) (buffer+1));
            break;
        case strip:
            set_strip(pixels + (STRIPSIZE*buffer[1]), (PIXEL*) (buffer+2));
            break;
        case dual_all:
            set_dual_all(pixels, (PIXEL*) (buffer+1), (PIXEL*) (buffer+4));
            break;
        case dual_strip:
            set_dual_strip(pixels + (STRIPSIZE*buffer[1]), (PIXEL*) (buffer+1), (PIXEL*) (buffer+4));
            break;
        case set:
            send_SPI(SPI_CH_1, led_buf, sizeof(led_buf));
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

void init_SPI(SPI_Config *spi_config, uint8 *spi_tx_buf)
{
    spi_config->which_spi = SPI_CH_1;
    spi_config->pb_clk = 15000000;
    spi_config->speed = 295000;
    spi_config->tx_en = 1;
    spi_config->clk_edge = 0;
    spi_config->tx_buffer_ptr = spi_tx_buf;
    spi_config->tx_buffer_size = sizeof(spi_tx_buf);
    initialize_SPI(*spi_config);
}
void init_UART(UART_Config *uart_config, uint8 *uart_rx_buffer)
{
    
}
void init_packet(Packetizer_Config *packet_config, UART_Config *uart_config)
{
    
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

//timer subroutine
void timer_tick ()
{
    switch (mode)
    {
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

#include "led.h"

void parse_packet(uint8 *buffer, uint8 size)
{
    static uint8 led_buf[NUMLEDS*3 + (NUMLEDS-1)/32 + 1] = {0};
    PIXEL *pixels = (PIXEL *) led_buf;

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

#include "led.h"

//LED frame buffer
uint8 led_buf[NUMLEDS*3 + (NUMLEDS-1)/32 + 1] = {0};

void parse_packet(uint8 *buffer, uint8 size)
{
    int i;
    T2CONbits.ON = 0;
    for (i = 0; i < size; i+=3)
    {
        led_buf[i] = (buffer[i+1] >> 1) | 0x80;
        led_buf[i+1] = (buffer[i] >> 1) | 0x80;
        led_buf[i+2] = (buffer[i+2] >> 1) | 0x80;
    }
    send_SPI(SPI_CH_1, led_buf, sizeof(led_buf));
}

void initialize_pins() {
    ANSELBbits.ANSB13 = 0;
    ANSELAbits.ANSA1 = 0;
    ANSELBbits.ANSB14 = 0;
    RPA1R = 0b0011; //set the RPA2R to SDO1
    U1RXR = 0b0011; // Set the pin to RPB13
}

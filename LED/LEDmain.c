/*
 * File:   LEDmain.c
 * Author: Nicholas Yeung
 *
 * Created on 2015-03-07 
 */
//insert configuration for new microcontrollers
#include <xc.h>

//select programming pins
#pragma config ICESEL = ICS_PGx1// ICE/ICD Comm Channel Select (Communicate on PGEC1/PGED1)

// DEVCFG3
// USERID = No Setting
#pragma config PMDL1WAY = OFF            // Peripheral Module Disable Configuration (Allow only one reconfiguration)
#pragma config IOL1WAY = OFF             // Peripheral Pin Select Configuration (Allow only one reconfiguration)
#pragma config FUSBIDIO = OFF            // USB USID Selection (Controlled by the USB Module)
#pragma config FVBUSONIO = OFF           // USB VBUS ON Selection (Controlled by USB Module)

// DEVCFG2
#pragma config FPLLIDIV = DIV_2         // PLL Input Divider (2x Divider)
#pragma config FPLLMUL = MUL_15         // PLL Multiplier (15x Multiplier)
#pragma config UPLLIDIV = DIV_12        // USB PLL Input Divider (12x Divider)
#pragma config UPLLEN = OFF             // USB PLL Enable (Disabled and Bypassed)
#pragma config FPLLODIV = DIV_2         // System PLL Output Clock Divider (PLL Divide by 1)

// DEVCFG1
#pragma config FNOSC = FRCPLL           // Oscillator Selection Bits (Fast RC Osc with PLL)
#pragma config FSOSCEN = OFF            // Secondary Oscillator Enable (Disabled)
#pragma config IESO = OFF               // Internal/External Switch Over (Disabled)
#pragma config POSCMOD = OFF            // Primary Oscillator Configuration (Primary osc disabled)
#pragma config OSCIOFNC = OFF            // CLKO Output Signal Active on the OSCO Pin (Enabled)
#pragma config FPBDIV = DIV_2           // Peripheral Clock Divisor (Pb_Clk is Sys_Clk/2)
#pragma config FCKSM = CSDCMD           // Clock Switching and Monitor Selection (Clock Switch Disable, FSCM Disabled)
#pragma config WDTPS = PS1048576        // Watchdog Timer Postscaler (1:1048576)
#pragma config WINDIS = OFF             // Watchdog Timer Window Enable (Watchdog Timer is in Non-Window Mode)
#pragma config FWDTEN = OFF             // Watchdog Timer Enable (WDT Disabled (SWDTEN Bit Controls))
#pragma config FWDTWINSZ = WINSZ_25     // Watchdog Timer Window Size (Window Size is 25%)

// DEVCFG0
#pragma config JTAGEN = OFF              // JTAG Enable (JTAG Port Enabled)
//programming pins are selected above
#pragma config PWP = OFF                // Program Flash Write Protect (Disable)
#pragma config BWP = OFF                // Boot Flash Write Protect bit (Protection Disabled)
#pragma config CP = OFF                 // Code Protect (Protection Disabled)

#include <stdio.h>
#include <stdlib.h>
#include "LED.h"
/*
 * 
 */
void delay(uint wait);
void colorWipe(uint c, uint8 wait) ;
void initialize_pins() {
    ANSELBbits.ANSB13 = 0;
    RPA1R = 0b0011; //set the RPA2R to SDO1
    U1RXR = 0b0011; // Set the pin to RPB13
}

int main(void) {
    uint8 spi_tx_buf[(NUMLEDS*3 + NUMLEDS/32 + 1)*3];
    uint8 uart_rx_buffer[200];
    //structures for configuring peripherals
    SPI_Config spi_config = {0};
    UART_Config uart_config = {0};
    Packetizer_Config packet_config = {0};
    //setup peripherals

    initialize_pins();//initialize pins on microcontroller

    spi_config.which_spi = SPI_CH_1;
    spi_config.pb_clk = 15000000;
    spi_config.speed = 295000;
    spi_config.tx_en = 1;
    spi_config.clk_edge = 0;
    spi_config.tx_buffer_ptr = spi_tx_buffer;
    spi_config.tx_buffer_size = sizeof(spi_tx_buffer);
    initialize_SPI(spi_config);

    uart_config.which_uart = UART_CH_1;
    uart_config.pb_clk = 15000000;
    uart_config.speed = 115200;
    uart_config.rx_buffer_ptr = uart_rx_buffer;
    uart_config.rx_buffer_size = sizeof(uart_rx_buffer);
    uart_config.rx_en = 1;

    packet_config.which_channel = PACKET_UART1;
    packet_config.control_byte = 0x0A;
    packet_config.callback = &process_recieve;
    packet_config.uart_config = uart_config;
    initialize_packetizer(packet_config);

    

    INTEnableSystemMultiVectoredInt();//enable interrupt
    
    asm volatile ("ei"); //reenable interrupts
    
    /*  Color(127, 127, 127); // White
        Color(127,   0,   0); // Red
        Color(127, 127,   0); // Yellow
        Color(  0, 127,   0); // Green
        Color(  0, 127, 127); // Cyan
        Color(  0,   0, 127); // Blue
        Color(127,   0, 127); // Violet*/
    while(1)
    {
        packetizer_background_process(PACKET_UART1);
    }
    return (EXIT_SUCCESS);
}

void packet_receive(uint8 *buffer, uint8 size)
{
    static PIXEL led_buffer[NUMLEDS] = {0};
    static uint8 zero_buf[NUMLEDS/32 + 1] = {0};
    int i;

    for(i = 0; i < NUMLEDS; i++)
            {
                led_buffer[i].green = buffer[1];
                led_buffer[i].red = buffer[2];
                led_buffer[i].blue = buffer[3];
            }
    send_SPI(SPI_CH_1, (uint8*) led_buffer, sizeof(led_buffer) + sizeof(zero_buf));
   
    /*switch(*buffer)
    {
        case manual:
            if((buffer[1]*32 + (size-2)/3) > NUMLEDS)
                break;
            memcpy(buffer+2, led_buffer+32*buffer[1], size-2);
            break;
        case all:
            for(i = 0; i < NUMLEDS; i++)
            {
                led_buffer[i].green = buffer[1];
                led_buffer[i].red = buffer[2];
                led_buffer[i].blue = buffer[3];
            }
            send_SPI(SPI_CH_1, (uint8*) led_buffer, sizeof(led_buffer) + sizeof(zero_buf));
            break;
            
        /*case strip:
            if(size < 5)
                break;
            for(i = 0; i < 32 && i < NUMLEDS-(buffer[1]*32); i++)
            {
                led_buffer[i] = buffer[2];
                led_buffer[i+1] = buffer[3];
                led_buffer[i+2] = buffer[4];
            }
            break;
        case dual:
            if(size < 8)
                break;
            for(i = 0; i < 16 && i < (NUMLEDS-(buffer[1]*32))/2; i++)
            {
                memcpy(led_buffer + i, buffer + 2, 6);
            }
            break;
         
        case set:
            send_SPI(SPI_CH_1, (uint8*) led_buffer, sizeof(led_buffer) + sizeof(zero_buf));
            break;
        default:
            break;
         
    }*/
                    
    
}

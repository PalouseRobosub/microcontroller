
/********************************************************
 *   File Name: main.c
 *
 *   Description:
 *              Main file
 *
 *
 *********************************************************/

/*************************************************************************
 Library Includes
 ************************************************************************/
#include "System.h"
#include "UART.h"
#include "packetizer.h"

/*************************************************************************
 Local Includes
 ************************************************************************/
#include "pneumatic.h"

/*************************************************************************
 System Defines
 ************************************************************************/
#define PB_CLK 15000000 //15 MHz

/*************************************************************************
 Processor Configuration
 ************************************************************************/
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


//forward declarations
void packetizer_callback(uint8* data, uint8 data_size);

/*************************************************************************
 Main Function
 ************************************************************************/
int main(void) {

    //buffer for uart ISRs
    uint8 uart_tx_buffer[128], uart_rx_buffer[128];

    //structures for configuring peripherals
    UART_Config uart_config = {0};
    Packetizer_Config packet_config = {0};


    //setup peripherals

    uart_config.which_uart = UART_CH_1;
    uart_config.pb_clk = PB_CLK;
    uart_config.speed = 115200;
    uart_config.rx_buffer_ptr = uart_rx_buffer;
    uart_config.rx_buffer_size = sizeof(uart_rx_buffer);
    uart_config.rx_en = 1;
    uart_config.tx_buffer_ptr = uart_tx_buffer;
    uart_config.tx_buffer_size = sizeof(uart_tx_buffer);
    uart_config.tx_en = 1;

    packet_config.which_channel = PACKET_UART1;
    packet_config.control_byte = 0x0A;
    packet_config.uart_config = uart_config;
    packet_config.callback = &packetizer_callback;
    initialize_packetizer(packet_config);

    //PPS setup
    ANSELA = ANSELB = 0;
    U1RXR = 0; //RPA2
    RPB4R = 1; //U1TX

    //setup pneumatics
    initialize_pneumatics();

    //Global interrupt enable. Do this last!
    INTEnableSystemMultiVectoredInt();
    asm volatile ("ei"); //reenable interrupts

    while (1) {
        //background process for processing received packets
        packetizer_background_process(PACKET_UART1);
    }

    return 0;
}

/*
 * currently this board expects the computer to send 2-byte packets
 * packet[0] = which pneumatic to control
 * packet[1] = whether selected pneumatic should be on(1) or off(0)
 * 
 */
void packetizer_callback(uint8* data, uint8 data_size)
{
    Pneumatic which_pneumatic;
    boolean enable;
    
    //confirm data size
    if (data_size >= 2)
    {
        which_pneumatic = data[0];
        enable = data[1];

        toggle_pneumatic(which_pneumatic, enable);
    }

}


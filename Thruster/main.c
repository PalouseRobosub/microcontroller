
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
#include "I2C.h"

/*************************************************************************
 Local Includes
 ************************************************************************/
#include "thruster.h"

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
void timer_callback(void);

/*************************************************************************
 Main Function
 ************************************************************************/
int main(void) {

    //buffer for uart ISRs
    uint8 uart_tx_buffer[128], uart_rx_buffer[128];
    uint8 i2c_tx_buffer[10*sizeof(I2C_Node)], i2c_rx_buffer[10*sizeof(I2C_Node)];

    //structures for configuring peripherals
    UART_Config uart_config = {0};
    Packetizer_Config packet_config = {0};
    I2C_Config i2c_config = {0};
    Timer_Config timer_config = {0};


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
    uart_config.rx_pin = Pin_RPB13;
    uart_config.tx_pin = Pin_RPB15;

    packet_config.which_channel = PACKET_UART_CH_1;
    packet_config.control_byte = 0x0A;
    packet_config.uart_config = uart_config;
    packet_config.callback = &packetizer_callback;
    initialize_packetizer(packet_config);
    
    i2c_config.callback = void;
    i2c_config.channel = I2C_CH_1;
    i2c_config.pb_clk = PB_CLK;
    i2c_config.rx_buffer_ptr = i2c_rx_buffer;
    i2c_config.rx_buffer_size = sizeof(i2c_rx_buffer);
    i2c_config.tx_buffer_ptr = i2c_tx_buffer;
    i2c_config.tx_buffer_size = sizeof(i2c_tx_buffer);
    initialize_I2C(i2c_config);
    
    timer_config.callback = &timer_callback;
    timer_config.enabled = TRUE;
    timer_config.frequency = 1;
    timer_config.pbclk = PB_CLK;
    timer_config.which_timer = Timer_1;
    


    //Global interrupt enable. Do this last!
    enable_Interrupts();

    while (1) {
        //background process for processing received packets
        bg_process_packetizer(PACKET_UART_CH_1);
    }

    return 0;
}

/*
 * currently this board expects the computer to send...
 * 
 */
void packetizer_callback(uint8* data, uint8 data_size)
{

}

void timer_callback(void)
{
    
}



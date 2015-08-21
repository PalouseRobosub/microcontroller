// <editor-fold defaultstate="collapsed" desc="main.c">
/*   File Name: main.c
 *
 *   Description:
 *              Main file
 *
 */
// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="System Includes">
#include "System.h"
#include "SPI.h"
#include "Timer.h"
#include "UART.h"
#include "packetizer.h"
#include "led.h"
#include "ledCommon.h"
// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="System Defines">
#define PB_CLK 15000000 //15 MHz
// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="Processor Configuration">
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
// </editor-fold>



/*************************************************************************
 Main Function
 ************************************************************************/
int main(void) {
    uint8 uart_rx_buffer[128];    
    uint8 spi_tx_buffer[NUMLEDS*3 + (NUMLEDS-1)/32 + 1];

    //structures for configuring peripherals
    UART_Config uart_config = {0};
    SPI_Config spi_config = {0};
    Packetizer_Config packet_config = {0};

    initialize_pins();

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

    packet_config.which_channel = PACKET_UART_CH_1;
    packet_config.control_byte = 0x0A;
    packet_config.callback = &parse_packet;
    packet_config.uart_config = uart_config;
    initialize_packetizer(packet_config);

    //Global interrupt enable. Do this lbm ast!
    INTEnableSystemMultiVectoredInt();
    asm volatile ("ei"); //reenable interrupts

    while (1) {
        //put background processes here
        packetizer_background_process(PACKET_UART_CH_1);
    }

    return 0;
}


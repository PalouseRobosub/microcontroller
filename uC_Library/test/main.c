
/********************************************************
 *   File Name: main.c
 *
 *   Description:
 *              Main file
 *
 *
 *********************************************************/

/*************************************************************************
 System Includes
 ************************************************************************/



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

/*************************************************************************
 Main Function
 ************************************************************************/
#include "../System.h"
#include "../Timer.h"
#include "../Queue.h"
#include "../UART.h"


void routine1();
void routine2();
void routine3();
void routine4();
void routine5();

void initialize_pins() {
    U1RXR = 0b0011; // Set the pin to RPB13
    RPB15R = 0b0001; //set the output pin to RPB15 U1TX queue
}

int main(void) {

    uint8 buffer_tx[100];
    uint8 buffer_rx[100];
    UARTConfig config = TX_EN | RX_EN;

    initialize_pins();

    TRISBbits.TRISB11 = 0;
    I2C1CON = 1;

    //testqueue = create_queue(buffer, 32);
    initialize_TIMER(Div_256, 50000, Timer_1, &routine1, 1);
    initialize_UART(9600, 15000000, UART1, buffer_rx, 100, buffer_tx, 100, config, NULL, NULL);

    //initialize_I2C(15000000, I2C1, buffer_rx, 100, buffer_tx, 100, NULL, NULL);
    //uint speed, uint pb_clk, Uart which_uart, uint8 *rx_buffer_ptr, uint8 rx_buffer_size,
                          // uint8 *tx_buffer_ptr, uint8 tx_buffer_size, boolean tx_en, boolean rx_en,
                          // void* rx_callback, void* tx_callback

    //Global interrupt enable. Do this last!
    INTEnableSystemMultiVectoredInt();
    asm volatile ("ei"); //reenable interrupts

    while (1) {
        //background tasks

    }

    return 0;
}

void routine1() {
    char c[10] = "coolstuff\n";
    send_UART(UART1, sizeof(char)*10, c);
    LATBbits.LATB11 = !LATBbits.LATB11;
    return;
}
void routine2() {
    int var = 0;
    var += 1;
    return;
}
void routine3() {
    int var = 0;
    var += 1;
    return;
}
void routine4() {
    int var = 0;
    var += 1;
    return;
}
void routine5() {
    int var = 0;
    var += 1;
    return;
}


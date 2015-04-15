
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
#include "../I2C.h"
#include "../packetizer.h"


void routine1();
void routine2(I2C_Node node);
void routine3();
void routine4();
void routine5();
void config_acl();

void initialize_pins() {
    ANSELA = ANSELB = 0;
    TRISB |= 1<<13;
    U1RXR = 0b0011; // Set the pin to RPB13
    RPB15R = 0b0001; //set the output pin to RPB15 U1TX queue
    TRISBCLR = 1<<7;
    LATBCLR = 1<<7;
}
uint8 buffer[10];
uint8 data[10] = "UART";

int main(void) {
    uint8 buffer_tx[100];
    uint8 buffer_rx[100];
    uint8 buffer_tx_u[100];
    uint8 buffer_rx_u[100];
    Timer_Config t_con;
    UART_Config u_con;
    Packetizer_Config p_con;
    I2C_Config i_con;

    //Configure the timer
    t_con.callback = &routine1;
    t_con.divide = Div_256;
    t_con.enabled = TRUE;
    t_con.period = 50000;
    t_con.which_timer = Timer_1;

    //Configure the UART
    u_con.pb_clk = 15000000;
    u_con.rx_buffer_ptr = buffer_rx;
    u_con.rx_buffer_size = 100;
    u_con.rx_callback = &routine3;
    u_con.rx_en = TRUE;
    u_con.speed = 9600;
    u_con.tx_buffer_ptr = buffer_rx;
    u_con.tx_buffer_size = 100;
    u_con.tx_callback = NULL;
    u_con.tx_en = TRUE;
    u_con.which_uart = UART_CH_1;

    //Configure the Packetizer
    p_con.callback = NULL;
    p_con.control_byte = '\t';
    p_con.uart_config = u_con;
    p_con.which_channel = UART_CH_1;

    //configure the I2C

    initialize_pins();

    initialize_timer(t_con);
    initialize_UART(u_con);
    initialize_packetizer(p_con);

    //Global interrupt enable. Do this last!
    INTEnableSystemMultiVectoredInt();
    asm volatile ("ei"); //reenable interrupts

    while (1) {
    }

    return 0;
}

 void config_acl(void)
 {

    //add the node to the queue
 }

void routine1() {
    //send_packet(UART_CH_1, data, 4);
}
void routine2(I2C_Node Node) {
    //this is the callback routine for I2C. In this routine, the data needs to be interpretted
    //data is sent in I2C 2's complement 16 bits

    //first interpret the information

    data[0] = 0xA;
    //send_UART(UART1, 1, data);
    //send_UART(UART1, 6, Node.data_buffer);
    //send_UART(UART1, 10, data);
    return;
}
void routine3() {
    int foo;
    foo++;
    send_packet(UART_CH_1, data, 4);
}
void routine4() {
}
void routine5() {
}



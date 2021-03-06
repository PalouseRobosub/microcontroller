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
#include "ADC.h"
#include "Timer.h"
#include "UART.h"
#include "I2C.h"
#include "Sensor.h"
#include "packetizer.h"
#include "sensor_setup.h"
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

//forward declarations
void timer_callback(void);
void sensor_send_uart(I2C_Node i2c_node);
void depth_callback(ADC_Node node);
void bat_volt_callback(ADC_Node node);
void read_switch(void);


/*************************************************************************
 Main Function
 ************************************************************************/
int main(void) {

    //buffer for uart ISRs
    uint8 uart_tx_buffer[128], uart_rx_buffer[128];
    u8 i2c_rx_buf[21*sizeof(I2C_Node)], i2c_tx_buf[21*sizeof(I2C_Node)];

    //structures for configuring peripherals
    UART_Config uart_config = {0};
    Timer_Config timer_config = {0};
    Packetizer_Config packet_config = {0};
    I2C_Config i2c_config = {0};

    volatile unsigned int i;

    TRISBbits.TRISB7 = 0;
    LATBbits.LATB7 = 0;
    ANSELBbits.ANSB15 = 0;
    RPB15R = 0x1;

    //start switch
    TRISBbits.TRISB2 = 1;
    ANSELBbits.ANSB2 = 0;
    
    //battery voltage
    TRISBbits.TRISB3 = 1;
    ANSELBbits.ANSB3 = 1;

    //setup peripherals
    timer_config.divide = Div_256;
    timer_config.period = 5859;
    timer_config.which_timer = Timer_1;
    timer_config.callback = &timer_callback;
    timer_config.enabled = 1;
    initialize_timer(timer_config);

    i2c_config.pb_clk = PB_CLK;
    i2c_config.channel = I2C_CH_1;
    i2c_config.tx_buffer_ptr = i2c_tx_buf;
    i2c_config.tx_buffer_size = sizeof(i2c_tx_buf);
    i2c_config.rx_buffer_ptr = i2c_rx_buf;
    i2c_config.rx_buffer_size = sizeof(i2c_rx_buf);
    initialize_I2C(i2c_config);

    uart_config.which_uart = UART_CH_1;
    uart_config.pb_clk = PB_CLK;
    uart_config.speed = 115200;
    uart_config.tx_buffer_ptr = uart_tx_buffer;
    uart_config.tx_buffer_size = sizeof(uart_tx_buffer);
    uart_config.tx_en = 1;
    uart_config.rx_buffer_ptr = uart_rx_buffer;
    uart_config.rx_buffer_size = sizeof(uart_tx_buffer);
    uart_config.rx_en = 1;    

    packet_config.control_byte = 0x0A;
    packet_config.which_channel = PACKET_UART1;
    packet_config.uart_config = uart_config;
    packet_config.callback = NULL;
    initialize_packetizer(packet_config);

    sensor_setup(&sensor_send_uart);

    //delay to allow sensors to power up before configuring
    for(i=0; i < 1000000; ++i);

    config_accel();
    config_gyro();
    config_mag();
    
 
    //Global interrupt enable. Do this last!
    INTEnableSystemMultiVectoredInt();
    asm volatile ("ei"); //reenable interrupts

    while (1) {
        //put background processes here
        bg_process_I2C();
        bg_process_ADC();
    }

    return 0;
}

void timer_callback(void)
{
    static ADC_Node depth_node = {0x01, ADC_CH_1, 0, &depth_callback};
    static ADC_Node bat_volt_node = {0x02, ADC_CH_5, 0 , &bat_volt_callback};

    //read all the sensors
    read_accel();
    read_gyro();
    read_mag();
    
    read_ADC(depth_node);
    read_ADC(bat_volt_node);

     
    //read the start switch
    read_switch();               
}

void sensor_send_uart(I2C_Node node)
{
    uint8 send_data[16];
    uint8 temp;
    uint8 i;

    send_data[0] = node.device_id;
    memcpy(&send_data[1], node.data_buffer, node.data_size);

    switch(node.device_id)
    {
        case SID_ACCELEROMETER_0:
            break;
        case SID_GYROSCOPE_0:
            temp = send_data[1];
            send_data[1] = send_data[2];
            send_data[2] = temp;

            temp = send_data[3];
            send_data[3] = send_data[4];
            send_data[4] = temp;

            temp = send_data[5];
            send_data[5] = send_data[6];
            send_data[6] = temp;
            break;
        case SID_MAGNETOMETER_0:      
            temp = send_data[1];
            send_data[1] = send_data[2];
            send_data[2] = temp;

            temp = send_data[3];
            send_data[3] = send_data[6];
            send_data[6] = temp;

            temp = send_data[5];
            send_data[5] = send_data[4];
            send_data[4] = temp;
            break;
        default:
            break;
    }


    i = 1+node.data_size;

    send_packet(PACKET_UART1, send_data, i);
}

void depth_callback(ADC_Node node)
{
    uint8 send_data[3];
    uint8 data_LB, data_HB;
    data_LB = node.data & 0xFF;
    data_HB = node.data >> 8;

    send_data[0] = SID_DEPTH_0;
    send_data[1] = data_LB;
    send_data[2] = data_HB;

    send_packet(PACKET_UART1, send_data, sizeof(send_data));
}

void bat_volt_callback(ADC_Node node)
{
    uint8 send_data[3];
    uint8 data_LB, data_HB;
    data_LB = node.data & 0xFF;
    data_HB = node.data >> 8;

    send_data[0] = SID_BAT_VOLT_0;
    send_data[1] = data_LB;
    send_data[2] = data_HB;

    send_packet(PACKET_UART1, send_data, sizeof(send_data));

}

void read_switch(void)
{
    uint8 send_data[2];

    send_data[0] = SID_START_SWITCH;
    send_data[1] = !(PORTBbits.RB2); //read the start switch, invert the logic

    send_packet(PACKET_UART1, send_data, sizeof(send_data));    
}
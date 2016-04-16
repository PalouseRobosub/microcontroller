#include "Sensors.h"
#include "sublibinal.h"
#define BUFF_SIZE 1024
//Global RX and TX buffers
uint8 rx[BUFF_SIZE], tx[BUFF_SIZE];

//Global I2C work and results buffer
uint8 work_ch_1[BUFF_SIZE], data_ch_1[128], results_ch_1[BUFF_SIZE];
uint8 work_ch_2[BUFF_SIZE], data_ch_2[128], results_ch_2[BUFF_SIZE];

void configureTimer()
{
    Timer_Config t = {0};
    t.callback = &timeToRead;
    t.enabled = FALSE;
    t.frequency = READ_RATE;
    t.pbclk = PB_CLK;
    t.which_timer = READ_TIMER;
    initialize_Timer(t);
    
    t.enabled = FALSE;
    t.frequency = 1667; //.6 ms
    t.pbclk = PB_CLK;
    t.which_timer = WAIT_TIMER_1;
    t.callback = &readDepth_1;
    initialize_Timer(t);
    
    t.which_timer = WAIT_TIMER_2;
    t.callback = &readDepth_2;
    initialize_Timer(t); //Initialize wait timer 2
}

void configureSerial()
{
    //Establish a serial communication 
    UART_Config u = {0};
    Packetizer_Config p = {0};
    u.speed = UART_SPEED;
    u.pb_clk = PB_CLK;
    u.rx_buffer_ptr = rx;
    u.rx_buffer_size = BUFF_SIZE;
    u.rx_en = TRUE;
    u.rx_pin = Pin_RPA4;
    u.tx_buffer_ptr = tx;
    u.tx_buffer_size = BUFF_SIZE;
    u.tx_en = TRUE;
    u.tx_pin = Pin_RPB4;
    u.which_uart = UART_CH_1;
    
    p.callback = NULL;
    p.control_byte = CONTROL_BYTE;
    p.uart_config = u;
    p.which_channel = PACKET_UART_CH_1;
    initialize_packetizer(p);
}

void configureI2C()
{
    I2C_Config i1 = {0}, i2 = {0};
    
    i1.channel = I2C_CH_1;
    i1.pb_clk = PB_CLK;
    i1.rx_buffer_ptr = results_ch_1;
    i1.rx_buffer_size = BUFF_SIZE;
    i1.tx_buffer_ptr = work_ch_1;
    i1.tx_buffer_size = BUFF_SIZE;
    initialize_I2C(i1);
    
    i2.channel = I2C_CH_2;
    i2.pb_clk = PB_CLK;
    i2.rx_buffer_ptr = results_ch_2;
    i2.rx_buffer_size = BUFF_SIZE;
    i2.tx_buffer_ptr = work_ch_2;
    i2.tx_buffer_size = BUFF_SIZE;
    //initialize_I2C(i2);
}
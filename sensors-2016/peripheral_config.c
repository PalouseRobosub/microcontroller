#include "Sensors.h"
#include "sublibinal.h"

//Global RX and TX buffers
uint8 rx[512], tx[512];

//Global I2C work and results buffer
uint8 work_ch_1[512], data_ch_1[512], results_ch_1[512];
uint8 work_ch_2[512], data_ch_2[512], results_ch_2[512];

void configureTimer()
{
    Timer_Config t = {0};
    t.callback = &timeToRead;
    t.enabled = FALSE;
    t.frequency = READ_RATE;
    t.pbclk = PB_CLK;
    t.which_timer = READ_TIMER;
    initialize_Timer(t);
    
    t.callback = &readDepth;
    t.enabled = FALSE;
    t.frequency = 1667; //.6 ms
    t.pbclk = PB_CLK;
    t.which_timer = WAIT_TIMER;
    initialize_Timer(t);
}

void configureSerial()
{
    //Establish a serial communication 
    UART_Config u = {0};
    Packetizer_Config p = {0};
    u.speed = UART_SPEED;
    u.pb_clk = PB_CLK;
    u.rx_buffer_ptr = rx;
    u.rx_buffer_size = sizeof(rx);
    u.rx_en = TRUE;
    u.rx_pin = Pin_RPA4;
    u.tx_buffer_ptr = tx;
    u.tx_buffer_size = sizeof(tx);
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
    i1.result_buffer_ptr = results_ch_1;
    i1.result_buffer_size = sizeof(results_ch_1);
    i1.work_buffer_ptr = work_ch_1;
    i1.work_buffer_size = sizeof(work_ch_1);
    i1.data_buffer_ptr = data_ch_1;
    i1.data_buffer_size = sizeof(data_ch_1);
    initialize_I2C(i1);
    
    i2.channel = I2C_CH_2;
    i2.pb_clk = PB_CLK;
    i2.result_buffer_ptr = results_ch_2;
    i2.result_buffer_size = sizeof(results_ch_2);
    i2.work_buffer_ptr = work_ch_2;
    i2.work_buffer_size = sizeof(work_ch_2);
    i2.data_buffer_ptr = data_ch_2;
    i2.data_buffer_size = sizeof(data_ch_2);
    initialize_I2C(i2);
}
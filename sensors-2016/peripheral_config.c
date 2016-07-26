#include "Sensors.h"
#include "sublibinal.h"

//Global RX and TX buffers
uint8 rx[UART_BUFF_SIZE], tx[UART_BUFF_SIZE];

//Global I2C work and results buffer
uint8 work_ch_1[BUFF_SIZE], data_ch_1[DATA_BUFF_SIZE], results_ch_1[BUFF_SIZE];

char toggleCount = 0;
void configureTimer()
{
    Timer_Config t = {0};
    t.enabled = FALSE;
    t.pbclk = PB_CLK;
    t.frequency = GYRO_ACCEL_FREQUENCY;
    t.which_timer = GYRO_ACCEL_TIMER;
    t.callback = &readGyroAccel;
    initialize_Timer(t);
    
    t.frequency = MAG_FREQUENCY;
    t.which_timer = MAG_TIMER;
    t.callback = &readMag;
    initialize_Timer(t);
    
    t.frequency = DEPTH_FREQUENCY; //~20ms overhead
    t.which_timer = DEPTH_TIMER;
    t.callback = &readDepth;
    initialize_Timer(t);
    
    t.which_timer = RESET_TIMER;
    t.frequency = GYRO_ACCEL_TIMER * .9;
    t.callback = &reset;
    initialize_Timer(t);
    
    t.enabled = FALSE;
    t.which_timer = TIMESTAMP_TIMER;
    t.callback = NULL;
    initialize_Timer(t);
    update_divider_Timer(TIMESTAMP_TIMER, Div_256);
    update_period_Timer(TIMESTAMP_TIMER, 65535);
}

void configureSerial()
{
    //Establish a serial communication 
    UART_Config u = {0};
    Packetizer_Config p = {0};
    u.speed = UART_SPEED;
    u.pb_clk = PB_CLK;
    u.rx_buffer_ptr = rx;
    u.rx_buffer_size = UART_BUFF_SIZE;
    u.rx_en = TRUE;
    u.rx_pin = Pin_RPA4;
    u.tx_buffer_ptr = tx;
    u.tx_buffer_size = UART_BUFF_SIZE;
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
    I2C_Config i1 = {0};
    
    i1.channel = I2C_CH_1;
    i1.pb_clk = PB_CLK;
    i1.result_buffer_ptr = results_ch_1;
    i1.result_buffer_size = BUFF_SIZE;
    i1.work_buffer_ptr = work_ch_1;
    i1.work_buffer_size = BUFF_SIZE;
    i1.data_buffer_ptr = data_ch_1;
    i1.data_buffer_size = DATA_BUFF_SIZE;
    initialize_I2C(i1);
}

inline void start_scl_reset()
{
    Timer_Config t = {0};
    
    disable_Timer(SCK_RST_TIMER);
    
    t.enabled = TRUE;
    t.which_timer = SCK_RST_TIMER;
    t.callback = &toggle;
    t.pbclk = PB_CLK;
    t.frequency = I2C_SPEED * 2;
    initialize_Timer(t);
}

inline void start_scl_watch()
{
    Timer_Config t = {0};
    
    disable_Timer(RESET_TIMER);
    
    t.callback = &reset;
    t.enabled = TRUE;
    t.frequency = GYRO_ACCEL_FREQUENCY * .9;
    t.which_timer = RESET_TIMER;
    t.pbclk = PB_CLK;
    initialize_Timer(t);
}
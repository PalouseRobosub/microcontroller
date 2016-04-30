#include "Sensors.h"
#include "sublibinal.h"
#define BUFF_SIZE 2048
#define UART_BUFF_SIZE 512
#define DATA_BUFF_SIZE 1024
//Global RX and TX buffers
uint8 rx[UART_BUFF_SIZE], tx[UART_BUFF_SIZE];

//Global I2C work and results buffer
uint8 work_ch_1[BUFF_SIZE], data_ch_1[DATA_BUFF_SIZE], results_ch_1[BUFF_SIZE];

char toggleCount = 0;
extern char contended;

void toggle()
{
    LATBINV = 1<<8;
    toggleCount++;
    if (toggleCount >= 20)
    {
        disable_Timer(SCK_RST);
        enable_Timer(RESET_TIMER);
        I2C1CONbits.ON = 1;
        TMR3 = TMR4 = 0;
        contended = 0;
        IFS1bits.I2C1MIF = 1; //Interrupt I2C to ensure that it runs
    }
}

void reset()
{
    //If this is called, we must send 9 clocks on the I2C clock line to clear bus contention
    I2C1CONbits.ON = 0;
    contended = 1;
    toggleCount = 0;
    enable_Timer(SCK_RST);
    disable_Timer(RESET_TIMER);
    LATBbits.LATB8 = 1; //output a 1
    TRISBbits.TRISB8 = 0;
}

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
    t.frequency = 40; //~20ms overhead
    t.pbclk = PB_CLK;
    t.which_timer = WAIT_TIMER;
    t.callback = &readDepth;
    initialize_Timer(t);
    
    t.which_timer = RESET_TIMER;
    t.frequency = READ_RATE * .9;
    t.callback = &reset;
    initialize_Timer(t);
    
    t.which_timer = SCK_RST;
    t.callback = &toggle;
    t.frequency = I2C_SPEED * 2;
    initialize_Timer(t);
    
    t.which_timer = Timer_5;
    t.callback = &readTemperature;
    t.frequency = 1;
    t.enabled = TRUE;
    //initialize_Timer(t);
    
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
    
    p.callback = &packetizerCallback;
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
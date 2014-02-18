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
#include "system.h"
#include "Sensors.h"
#include "I2C_ISR.h"
#include "Timer_ISR.h"
#include "comm_UART_ISR.h"
#include "motor_UART_ISR.h"
#include "ADC_ISR.h"
#include "LED_SPI_ISR.h"


/*************************************************************************
 Processor Configuration
 ************************************************************************/
#pragma config FPLLMUL = MUL_20, FPLLIDIV = DIV_2, FPLLODIV = DIV_1, FWDTEN = OFF
#pragma config POSCMOD = HS, FNOSC = PRIPLL, FPBDIV = DIV_8
#pragma config ICESEL = ICS_PGx2

 
/*************************************************************************
 Main Function
 ************************************************************************/
int main(void)
{    

    //setup/configure hardware modules
    timer_1_setup();
    i2c_bank_0_setup();
    comm_uart_setup();
    motor_uart_setup();
    adc_setup();
    led_spi_setup();

    //load nodes onto queues to initialize sensors
    i2c_ACL_Initialize();
    i2c_GYRO_Initialize();


    //start each ISR
    i2c_bank_0_begin();    
    motor_uart_begin();
    comm_uart_begin();
    led_spi_begin();

    //Global interrupt enable. Do this last!
    INTEnableSystemMultiVectoredInt();
    INTEnableInterrupts();

    while (1)
    {
        //insert background tasks
        bg_process_comm_uart();
    }

    return 0;
}





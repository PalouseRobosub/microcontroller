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
#if defined(COMPILE_OLD_SUB)
#pragma config FPLLMUL = MUL_20, FPLLIDIV = DIV_2, FPLLODIV = DIV_1, FWDTEN = OFF
#pragma config POSCMOD = HS, FNOSC = PRIPLL, FPBDIV = DIV_8
#pragma config ICESEL = ICS_PGx2

#elif defined(COMPILE_SENSOR_BOARD) || \
    defined (COMPILE_THRUSTER_BOARD) || \
    defined (COMPILE_LED_BOARD) || \
    defined (COMPILE_ACTUATION_BOARD)

//insert configuration for new microcontrollers

#endif

 
/*************************************************************************
 Main Function
 ************************************************************************/
int main(void)
{    

#if defined (COMPILE_OLD_SUB)
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

#elif defined (COMPILE_SENSOR_BOARD)
    //setup/configure hardware modules
    timer_1_setup();
    i2c_bank_0_setup();
    comm_uart_setup();
    adc_setup();

    //load nodes onto queues to initialize sensors
    i2c_ACL_Initialize();
    i2c_GYRO_Initialize();

    //start each ISR
    i2c_bank_0_begin();
    comm_uart_begin();

#elif defined (COMPILE_THRUSTER_BOARD)
    //setup/configure hardware modules

    //load nodes onto queues to initialize sensors

    //start each ISR


#elif defined (COMPILE_LED_BOARD)
    //setup/configure hardware modules
    comm_uart_setup();
    led_spi_setup();

    //load nodes onto queues to initialize sensors

    //start each ISR
    comm_uart_begin();
    led_spi_begin();

#elif defined (COMPILE_ACTUATION_BOARD)
    //setup/configure hardware modules
    timer_1_setup();
    comm_uart_setup();

    //load nodes onto queues to initialize sensors

    //start each ISR
    comm_uart_begin();

#endif

    //Global interrupt enable. Do this last!
    INTEnableSystemMultiVectoredInt();
    INTEnableInterrupts();

    while (1)
    {
        //background tasks

#if defined (COMPILE_OLD_SUB)
        bg_process_comm_uart();
        
#elif defined (COMPILE_SENSOR_BOARD)
        //insert background tasks (if necessary)
#elif defined (COMPILE_THRUSTER_BOARD)
        //insert background tasks (if necessary)
#elif defined (COMPILE_LED_BOARD)
        //insert background tasks (if necessary)
#elif defined (COMPILE_ACTUATION_BOARD)
        //insert background tasks (if necessary)
#endif


    }

    return 0;
}


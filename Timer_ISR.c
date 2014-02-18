/********************************************************
 *   File Name: Timer_ISR.c
 *
 *   Description:
 *              source code file for the Timer ISR
 *
 *
 *********************************************************/

/*************************************************************************
 System Includes
 ************************************************************************/
#include "system.h"
#include "Sensors.h"
#include "Timer_ISR.h"
#include "peripheral/timer.h"
#include "I2C_ISR.h"
#include "comm_UART_ISR.h"
#include "motor_UART_ISR.h"
#include "ADC_ISR.h"
#include "LED_SPI_ISR.h"


/*************************************************************************
 Variables
 ************************************************************************/

/********************************************************
 *   Function Name:
 *
 *   Description:
 *
 *
 *********************************************************/
void timer_1_setup(void)
{
    OpenTimer1(T1_ON | T1_SOURCE_INT | T1_PS_1_256, 0xF40);

    IPC1bits.T1IP = 7;
    IEC0bits.T1IE = 1;
}

/********************************************************
 *   Function Name:
 *
 *   Description:
 *
 *
 *********************************************************/
inline void timer_1_begin(void)
{
    IFS0bits.T1IF = 1; //set the interrupt flag
}

/********************************************************
 *   Function Name:
 *
 *   Description:
 *
 *
 *********************************************************/
void __ISR(_TIMER_1_VECTOR, IPL7AUTO) Timer1Handler(void)
{
    extern boolean I2C_BANK_0_is_idle;
    extern boolean COMM_UART_is_idle;
    extern boolean ADC_is_idle;

    INTDisableInterrupts();

    i2c_GYRO_Read();
    i2c_ACL_Read();


    /*
    this creating and sending a uart node below should
    not be necessary, but I remember removing it
    broke the uart. This needs to be debugged.
     */
    comm_uart_CreateNode( 'A', 'B', 'C' );
    if (COMM_UART_is_idle)
    {
        comm_uart_begin();
    }
    
    if (I2C_BANK_0_is_idle)
    {
        i2c_bank_0_begin();
    }

    ADC_Depth_Read();
    ADC_Battery_Read();
    if (ADC_is_idle)
    {
        adc_begin();
    }
    
        
    IFS0bits.T1IF = 0; //clear the interrupt flag
    INTEnableInterrupts();
}



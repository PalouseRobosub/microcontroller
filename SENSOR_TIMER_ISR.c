/********************************************************
 *   File Name: SENSOR_TIMER_ISR.c
 *
 *   Description:
 *              source code file for the Sensor Timer ISR
 *
 *
 *********************************************************/

/*************************************************************************
 System Includes
 ************************************************************************/
#include "system.h"

#if defined (COMPILE_OLD_SUB) || defined (COMPILE_SENSOR_BOARD)
#include "Sensors.h"
#include "SENSOR_TIMER_ISR.h"
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
void sensor_timer_setup(void) {
    SENSOR_TIMER_PS = SENSOR_TIMER_PRESCALER; //set the prescaler bits (1/256 in this case)
    SENSOR_TIMER_PR = SENSOR_TIMER_PERIOD_REGISTER; //set the period register

    SENSOR_TIMER_INT_PRIORITY_set(7); //set the priority
    SENSOR_TIMER_INT_set(1); //enable the interrupt

    SENSOR_TIMER_EN = 1; //enable the timer
}

/********************************************************
 *   Function Name:
 *
 *   Description:
 *
 *
 *********************************************************/
inline void sensor_timer_begin(void) {
    SENSOR_TIMER_IF = 1; //set the interrupt flag
}

/********************************************************
 *   Function Name:
 *
 *   Description:
 *
 *
 *********************************************************/
void __ISR(_SENSOR_TIMER_VECTOR, IPL7AUTO) sensor_timer_handler(void) {
    extern boolean I2C_BANK_0_is_idle;
    extern boolean COMM_UART_is_idle;
    extern boolean ADC_is_idle;

    INTDisableInterrupts();


    i2c_GYRO_Read();
    i2c_ACL_Read();
    i2c_MAG_Read();


    /*
    this creating and sending a uart node below should
    not be necessary, but I remember removing it
    broke the uart. This needs to be debugged.
     */
    comm_uart_CreateNode('A', 'B', 'C');
    if (COMM_UART_is_idle) {
        comm_uart_begin();
    }

    if (I2C_BANK_0_is_idle) {
        i2c_bank_0_begin();
    }

    ADC_Depth_Read();
    ADC_Battery_Read();
    if (ADC_is_idle) {
        adc_begin();
    }



    IFS0bits.T1IF = 0; //clear the interrupt flag
    INTEnableInterrupts();
}


#endif
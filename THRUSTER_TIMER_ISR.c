/********************************************************
 *   File Name: THRUSTER_TIMER_ISR.c
 *
 *   Description:
 *              source code file for the Thruster Timer ISR
 *
 *
 *********************************************************/

/*************************************************************************
 System Includes
 ************************************************************************/
#include "system.h"
#if defined (COMPILE_THRUSTER_BOARD)

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
void thruster_timer_setup(void) {
    THRUSTER_TIMER_PS = 0b11; //set the prescaler bits (1/256 in this case)
    THRUSTER_TIMER_PR = 0xF40; //set the period register

    THRUSTER_TIMER_INT_PRIORITY_set(7); //set the priority
    THRUSTER_TIMER_INT_set(1); //enable the interrupt

    THRUSTER_TIMER_EN = 1; //enable the timer
}

/********************************************************
 *   Function Name:
 *
 *   Description:
 *
 *
 *********************************************************/
inline void thruster_timer_begin(void) {
    THRUSTER_TIMER_IF = 1; //set the interrupt flag
}

/********************************************************
 *   Function Name:
 *
 *   Description:
 *
 *
 *********************************************************/
void __ISR(_THRUSTER_TIMER_VECTOR, IPL7AUTO) thruster_timer_handler(void) {

    extern boolean COMM_UART_is_idle;


    INTDisableInterrupts();






    IFS0bits.T1IF = 0; //clear the interrupt flag
    INTEnableInterrupts();
}

#endif

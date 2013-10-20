/********************************************************
 *   File Name: Timer_ISR.c
 *
 *   Description:
 *              source code file for the Timer ISR
 *
 *
 *********************************************************/
#include "functions.h"
#include "Sensors.h"
#include "Timer_ISR.h"
#include "peripheral/timer.h"
#include "I2C_ISR.h"
#include "UART_ISR.h"


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
    OpenTimer1(T1_ON | T1_SOURCE_INT | T1_PS_1_256, 0xff0);

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
    extern boolean I2C1_is_idle;
    INTDisableInterrupts();

    PORTGbits.RG1 = !PORTGbits.RG1; //for testing, remove in final code
    
        i2c_GYRO_Read();
        i2c_ACL_Read();
        if (I2C1_is_idle)
        {
            i2c_1_begin();
        }

    IFS0bits.T1IF = 0; //clear the interrupt flag
    INTEnableInterrupts();
}
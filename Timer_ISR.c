/********************************************************
 *   File Name: Timer_ISR.c
 *
 *   Description:
 *              source code file for the Timer ISR
 *
 *
 *********************************************************/
#include "functions.h"
#include "Timer_ISR.h"
#include "peripheral/timer.h"
#include "I2C_ISR.h"


/*************************************************************************
 Variables
 ************************************************************************/

/********************************************************
 *   Function Name: timer_1_setup()
 *
 *   Description: Setup the timer 1 (clock speeds and interrupt priority)
 *
 *
 *********************************************************/
void timer_1_setup(void)
{
    OpenTimer1(T1_ON | T1_SOURCE_INT | T1_PS_1_256, 0x0FF);

    IEC0bits.T1IE = 1;
    IPC1bits.T1IP = 2;

}

/********************************************************
 *   Function Name: timer_1_begin()
 *
 *   Description: Set the timer 1 interrupt flag (eneable the interrupt)
 *
 *
 *********************************************************/
inline void timer_1_begin(void)
{
    IFS0bits.T1IF = 1; //set the interrupt flag
}

/********************************************************
 *   Function Name: Timer1Handler()
 *
 *   Description: ISR for Timer 1
 *
 *
 *********************************************************/
void __ISR(_TIMER_1_VECTOR,ipl2) Timer1Handler(void)
{
    static uint count;
    IFS0bits.T1IF = 0; //clear the interrupt flag
    count++;
    if (count == 20)
    {
        i2c_ACL_Read();
        //i2c_1_begin();
        count = 0;
    }
}
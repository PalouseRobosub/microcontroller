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
void __ISR(_TIMER_1_VECTOR,ipl2) Timer1Handler(void)
{
    IFS0bits.T1IF = 0; //clear the interrupt flag
    
}
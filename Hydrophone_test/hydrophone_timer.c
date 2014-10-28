/********************************************************
 *   File Name: hydrophone_timer.c
 *
 *   Description:
 *              source code file for the timer for the hydrophones
 *
 *
 *********************************************************/


/*************************************************************************
 System Includes
 ************************************************************************/
#include "system.h"
#include "hydrophone_timer.h"

/*************************************************************************
 Variables
 ************************************************************************/


/********************************************************
 *   Function Name: hydrophone_timer_setup()
 *
 *   Description:
 *
 *
 *********************************************************/
void hydrophone_timer_setup(void) {

    T1CON = 0; //Clear all bits in register

    T1CONbits.TCS = 0; //Internal Peripheral Clock
    T1CONbits.TCKPS = 0b00; //Prescale Value 1:1

    PR1 = 0xffff; //Cycles before and interupt is triggered
    //Need to adjust for PB_Clk Speed
    //Should give enough time for all hydrophones to trigger
    //and trigger before the next ping

    TMR1 = 0; //Set Timer Register to zero;

    return 0;
}



/********************************************************
 *   Function Name: hydrophone_timer_handler()
 *
 *   Description:
 *
 *
 *********************************************************/
void __ISR(_TIMER_1_VECTOR, IPL7AUTO) hydrophone_timer_handler(void) {
    
    INTDisableInterrupts();




    INTEnableInterrupts();

}


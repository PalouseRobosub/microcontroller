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

    IFS0bits.T1IF = 0;
    IEC0bits.T1IE = 1;

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
    
    IFS0bits.T1IF = 0; //Set flag to zero

    //Reset the registers
    s0 = 0;
    s1 = 0;
    s2 = 0;
    s3 = 0;
    s4 = 0;

    //Reset instance
    s5 = 0;

    //Set register for error message
    s6 = 1;

    INTEnableInterrupts();

}


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


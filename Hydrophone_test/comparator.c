/********************************************************
 *   File Name: comparator.c
 *
 *   Description:
 *              source code file for the comparator module
 *
 *
 *********************************************************/


/*************************************************************************
 System Includes
 ************************************************************************/
#include "system.h"
#include "comparator.h"

/*************************************************************************
 Variables
 ************************************************************************/


/********************************************************
 *   Function Name: comparator_setup()
 *
 *   Description:
 *
 *
 *********************************************************/
void comparator_setup(void) {

}



/********************************************************
 *   Function Name: comparator_handler()
 *
 *   Description:
 *
 *
 *********************************************************/
//you can have all three interrupts map to the same vector
//void __ISR((_COMPARATOR_1_VECTOR, _COMPARATOR_2_VECTOR, _COMPARATOR_3_VECTOR), IPL7AUTO) comparator_handler(void) {
//or you could create individual ISRs for each interrupt
void __ISR(_COMPARATOR_1_VECTOR, IPL7AUTO) comparator_handler(void) {

    INTDisableInterrupts();


    

    INTEnableInterrupts();

}

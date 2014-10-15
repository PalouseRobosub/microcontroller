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
void __ISR(_COMPARATOR_1_VECTOR, IPL7AUTO) comparator_handler(void) {
    

    INTDisableInterrupts();


    

    INTEnableInterrupts();

}

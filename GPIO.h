/********************************************************
 *   File Name: GPIO.h
 *
 *   Description:
 *              header file for the ADC ISR
 *
 *
 *********************************************************/

#ifndef GPIO_H
#define	GPIO_H

#include "system.h"


/*************************************************************************
 System Includes
 ************************************************************************/

/*************************************************************************
 Typedefs
 ************************************************************************/

/*************************************************************************
 Constants
 ************************************************************************/

/*************************************************************************
 Structure Definitions
 ************************************************************************/


/*************************************************************************
 Enums
 ************************************************************************/


/*************************************************************************
 Variables
 ************************************************************************/

/*************************************************************************
 Function Declarations
 ************************************************************************/

 /********************************************************
 *   Function Name:
 *
 *   Description:
 *
 *
 *********************************************************/

 /********************************************************
 *   Function Name: GPIO_setup(void)
 *
 *   Description:
 *                  sets up the TRIS registers
 *                  for all GPIO pins
 *
 *********************************************************/
void GPIO_setup(void);

void sys_init(void);

int stepper_state_machine(int dir, int which_stepper);

void output_to_stepper_motor(int command, int which_stepper);

#endif	/* GPIO_H */


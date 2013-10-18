/********************************************************
 *   File Name: functions.h
 *
 *   Description:
 *              Main header file
 *
 *
 *********************************************************/

#ifndef FUNCTIONS_H
#define	FUNCTIONS_H


/*************************************************************************
 System Includes
 ************************************************************************/
//#include <plib.h> //includes absolutely everything
#include <peripheral/ports.h>
#include <peripheral/i2c.h>



/*************************************************************************
 Typedefs
 ************************************************************************/
typedef unsigned int uint;
typedef unsigned char uint8;
typedef uint8 boolean;

/*************************************************************************
 Constants
 ************************************************************************/
#define SYS_CLK_FREQ (40000000) //40MHz CLK

#define QueueSize 15

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
 *   Function Name: read_buttons()
 *
 *   Description: Reads buttons from board
 *
 *
 *********************************************************/
unsigned int read_buttons(void);

 /********************************************************
 *   Function Name: write_leds(unsigned int)
 *
 *   Description: writes to LED's on board
 *
 *
 *********************************************************/
void write_leds(uint8 value);

 void delay(void);

#endif	/* FUNCTIONS_H */


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
#define TRUE  (1)
#define FALSE (0)
#define I2C_MAX_DATA_SIZE 3
#define I2C_MAX_ADDR_SIZE 2

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
 *   Function Name:
 *
 *   Description:
 *
 *
 *********************************************************/
unsigned int read_buttons(void);

 /********************************************************
 *   Function Name:
 *
 *   Description:
 *
 *
 *********************************************************/
void write_leds(unsigned int);

 void delay(void);

#endif	/* FUNCTIONS_H */


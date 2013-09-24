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


typedef struct i2c_node
{
    uint8 device_address; //device to read from
    uint8 sub_address[I2C_MAX_ADDR_SIZE]; //sub address(s) to read from
    uint8 sub_address_size; //size (in bytes) of the sub address
    boolean write_nread; //determine if we are reading or writing data
    uint8 tx_data[I2C_MAX_DATA_SIZE]; //data to be written (if using write mode)
    uint8 data_size; //number of bytes expected to write/read
   struct i2c_node*   next;
}I2C_Node;

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

 

 /********************************************************
 *   Function Name:
 *
 *   Description:
 *
 *
 *********************************************************/



//  |=                                                |=
// /\                                                /\
//|  |                                              |  |
//|  |                                              |  |
//|  | ^  ^  ^  ^  ^  ^  ^  ^  ^  ^  ^  ^  ^  ^  ^  |  |
//======================================================
//=============FORTRESS OF THE LINK MASTER==============
//==================(NO GIRLS ALLOWED)==================
//======================================================




//======================================================
//=============FORTRESS OF THE LINK MASTER==============
//========http://orteil.dashnet.org/cookieclicker/ =====
//======================================================

#endif	/* FUNCTIONS_H */


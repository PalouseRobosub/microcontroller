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

/*************************************************************************
 Constants
 ************************************************************************/
#define SYS_CLK_FREQ (40000000) //40MHz CLK
#define I2C_CLK_FREQ (100000) //100kHz CLK

/*************************************************************************
 Structure Definitions
 ************************************************************************/
typedef struct node
{
    int data;
   struct node*   next;    // may be repeated for different data structures.
}Node;

typedef struct i2c_node
{
    uint8 device_address; //device to read from
    uint8 device_sub_address; //sub address to read from
    uint8 receive_size; //number of bytes expected back
   struct i2c_node*   next;
}I2C_Node;

/*************************************************************************
 Enums
 ************************************************************************/
typedef enum //state of the I2C bus
{
    IDLE,
    SENDING_START,
    SELECTING_DEVICE_W,
    WRITING_SUB_ADDR,
    SENDING_RESTART,
    SELECTING_DEVICE_R,
    RECEIVING_DATA,
    SENDING_STOP


} I2C_STATE;



/*************************************************************************
 Variables
 ************************************************************************/
I2C_STATE i2c_state;

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
void setup_i2c (I2C_MODULE i2c_id);

 /********************************************************
 *   Function Name:
 *
 *   Description:
 *
 *
 *********************************************************/
void i2c_1_isr(void);

 /********************************************************
 *   Function Name:
 *
 *   Description:
 *
 *
 *********************************************************/
extern int i2c_status;

/********************************************************
 *   Function Name:
 *
 *   Description:
 *
 *
 *********************************************************/
Node* makeNode( int item );

/********************************************************
 *   Function Name:
 *
 *   Description:
 *
 *
 *********************************************************/
void freeNode( Node*   thisNode );




//======================================================
//=============FORTRESS OF THE LINK MASTER==============
//==================(NO GIRLS ALLOWED)==================
//======================================================




//======================================================
//=============FORTRESS OF THE LINK MASTER==============
//========http://orteil.dashnet.org/cookieclicker/ =====
//======================================================

#endif	/* FUNCTIONS_H */


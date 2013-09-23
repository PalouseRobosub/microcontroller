/* 
 * File:   functions.h
 * Author: James
 *
 * Created on September 21, 2013, 8:06 PM
 */

#ifndef FUNCTIONS_H
#define	FUNCTIONS_H


//System Includes
#include <peripheral/ports.h>
#include <peripheral/i2c.h>
//#include <plib.h> //include everything


//macros
#define uint unsigned int

//Constants
#define SYS_CLK_FREQ (40000000) //40MHz CLK
#define I2C_CLK_FREQ (100000) //100kHz CLK


//Function Declarations
unsigned int read_buttons(void);
void write_leds(unsigned int);

void setup_i2c (I2C_MODULE i2c_id);

void i2c_1_isr(void);

extern int i2c_status;





//======================================================
//=============FORTRESS OF THE LINK MASTER==============
//==================(NO GIRLS ALLOWED)==================
//======================================================
struct node
{
    int data;
   struct node*   next;    // may be repeated for different data structures.
};
typedef struct node Node;
Node* makeNode( int item );
void freeNode( Node*   thisNode );

//======================================================
//=============FORTRESS OF THE LINK MASTER==============
//========http://orteil.dashnet.org/cookieclicker/ =====
//======================================================

#endif	/* FUNCTIONS_H */


/********************************************************
 *   File Name: functions.c
 *
 *   Description:
 *              Main source code file
 *
 *
 *********************************************************/

#include "functions.h"

 /********************************************************
 *   Function Name:
 *
 *   Description:
 *
 *
 *********************************************************/
unsigned int read_buttons(void)
{
    unsigned int buttons = 0;

    buttons = PORTRead(IOPORT_A) & (BIT_6 | BIT_7);

    buttons = buttons << 4;
}

 /********************************************************
 *   Function Name:
 *
 *   Description:
 *
 *
 *********************************************************/
void write_leds(unsigned int current_state)
{
    PORTWrite(IOPORT_B, current_state);
}

 /********************************************************
 *   Function Name:
 *
 *   Description:
 *
 *
 *********************************************************/
void setup_i2c (I2C_MODULE i2c_id)
{

    I2CSetFrequency(i2c_id, SYS_CLK_FREQ, I2C_CLK_FREQ);
    I2CEnable(i2c_id, TRUE);

    return;
}

 /********************************************************
 *   Function Name:
 *
 *   Description:
 *
 *
 *********************************************************/
void i2c_1_isr(void)
{
    
    uint i2c_status;
    
    i2c_status = I2CGetStatus(I2C1);

    if ((i2c_status & I2C_START) == I2C_START)
    {
        //start condition was just sent, add code for sending

    }
    else if ((i2c_status & I2C_START) == I2C_START)
    {

    }

}




//======================================================
//=============FORTRESS OF THE LINK MASTER==============
//==================(NO GIRLS ALLOWED)==================
//======================================================
 /********************************************************
 *   Function Name:
 *
 *   Description:
 *
 *
 *********************************************************/
Node* makeNode( int item )
{
   Node*    tmp;

   tmp         =  (Node*)malloc( sizeof ( Node )  );
   tmp->data  =  item;
   tmp->next  =  NULL;

   return   tmp;
}
  /********************************************************
 *   Function Name:
 *
 *   Description:
 *
 *
 *********************************************************/
void freeNode( Node* tmp )
{
   if ( NULL != tmp->next )
   {
      freeNode( tmp->next );
   }
   free( tmp );
}

//======================================================
//=============FORTRESS OF THE LINK MASTER==============
//========http://orteil.dashnet.org/cookieclicker/ =====
//======================================================
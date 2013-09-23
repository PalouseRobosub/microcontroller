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
    //variables
    extern I2C_STATE i2c_state;
    I2C_STATUS current_status;
    static I2C_Node* current_sensor;
    uint8 received_data;

    //disable interrupts
    //<code to disable interrupts>

    //clear interrupt flag
    //<code to clear interrupt flag>

    //get I2C status
    current_status = I2CGetStatus(I2C1);


    switch (i2c_state)
    {
        case IDLE:
            break;


        case SENDING_START:
            //current_sensor = <dequeue node function>
                if (current_sensor == NULL)  //error, queue is empty!
                {
                    return;
                }
            I2CStart(I2C1);
            i2c_state = SELECTING_DEVICE_W;
            break;


        case SELECTING_DEVICE_W:
            //confirm start signal was sent
            if ((current_status & I2C_START) == I2C_START)
            {
                //shifting the address left and adding the write bit
                I2CSendByte(I2C1, ((current_sensor->device_address << 1) & 0xFE));
                i2c_state = WRITING_SUB_ADDR;
            }
            else
            {
                //error, start signal was not sent
            }
            break;
        case WRITING_SUB_ADDR:
            //confirm the device acknowledged the hail
            if ((current_status & I2C_BYTE_ACKNOWLEDGED) == I2C_BYTE_ACKNOWLEDGED)
            {
                I2CSendByte(I2C1, current_sensor->device_sub_address);
                i2c_state = SENDING_RESTART;
            }
            else
            {
                //error, device did not acknowledge the hail
            }
            break;
        case SENDING_RESTART:
             //confirm the device acknowledged sub address
            if ((current_status & I2C_BYTE_ACKNOWLEDGED) == I2C_BYTE_ACKNOWLEDGED)
            {
                I2CRepeatStart(I2C1)
                i2c_state = SELECTING_DEVICE_R;
            }
            else
            {
                //error, device did not acknowledge the sub address
            }
            break;
        case SELECTING_DEVICE_R:
             //confirm restart signal was sent
            if ((current_status & I2C_START) == I2C_START)
            {
                //shifting the address left and adding the write bit
                I2CSendByte(I2C1, ((current_sensor->device_address << 1) | 0x01));
                i2c_state = RECEIVING_DATA;
            }
            else
            {
                //error, restart signal was not sent
            }
            break;
        case RECEIVING_DATA:
            //confirm the device acknowledged the hail
            if ((current_status & I2C_BYTE_ACKNOWLEDGED) == I2C_BYTE_ACKNOWLEDGED) 
            { 
                received_data = I2CGetByte(I2C1);
                i2c_state = SENDING_STOP;
            }
            else
            {
                //error, device did not acknowledge the hail
            }
            break;
        case SENDING_STOP:
            I2CStop(I2C1);
            i2c_state = IDLE;
            break;
        default:
            break;
            //error
    }

    

    //re-enable interrupts
    return;
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

   //tmp         =  (Node*)malloc( sizeof ( Node )  );
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
   //free( tmp );
}

//======================================================
//=============FORTRESS OF THE LINK MASTER==============
//========http://orteil.dashnet.org/cookieclicker/ =====
//======================================================
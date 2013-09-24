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

 




//======================================================
//=============FORTRESS OF THE LINK MASTER==============
//==================(NO GIRLS ALLOWED)==================
//======================================================
 /********************************************************
 *   Function Name: InitializeQueue()
 *
 *   Description: Clears the queue and resets parameters
 *
 *
 *********************************************************/
void InitializeQueue( Queue* root )
{
    uint i=0;
    for (i=0;i<QueueSize;i++)
    {
        root->I2CQueue[i].data = 0;
    }
    root->QueueLength = 0;
    root->QueueStart = 0;
    root->QueueEnd = 0;
}
 /********************************************************
 *   Function Name:
 *
 *   Description:
 *
 *
 *********************************************************/
int addToQueue( Queue* root, int item )
{
    if (root->QueueEnd == root->QueueStart && root->QueueLength > 0)
    {
        return 1; //Error, would overwrite start of list
    }
    root->I2CQueue[root->QueueEnd].data = item;
    root->QueueLength++;
    if (root->QueueEnd == QueueSize-1)
    {
        root->QueueEnd = 0;
    }
    else
    {
        root->QueueEnd++;
    }
    return 0;
}
  /********************************************************
 *   Function Name:
 *
 *   Description:
 *
 *
 *********************************************************/
int freeNode( Queue* root, Node* Free )
{
    if (root->QueueLength == 0)
    {
        return 1; //Can't read from queue if empty
    }
    *Free = (root->I2CQueue[root->QueueStart]); //Returns the Node
    if (root->QueueStart == QueueSize-1)
    {
        root->QueueStart = 0;
    }
    else
    {
        root->QueueStart++;
    }
    root->QueueLength--;
    return 0;


}

//======================================================
//=============FORTRESS OF THE LINK MASTER==============
//========http://orteil.dashnet.org/cookieclicker/ =====
//======================================================
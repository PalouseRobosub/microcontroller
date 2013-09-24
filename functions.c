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
 *   Description: Initiallizes a blank queue.  Returns pointer to array
 *
 *
 *********************************************************/
void InitializeQueue( Node root[] )
{
    uint i=0;
    for (i=0;i<QueueSize;i++)
    {
        root[i].data = 0;
    }
    QueueLength = 0;
}
 /********************************************************
 *   Function Name:
 *
 *   Description:
 *
 *
 *********************************************************/
int addToQueue( Node Queue[], int item )
{
    if (QueueEnd == QueueStart && QueueLength > 0)
    {
        return 1; //Error, would overwrite start of list
    }
    Queue[QueueEnd].data = item;
    QueueLength++;
    if (QueueEnd == QueueSize-1)
    {
        QueueEnd = 0;
    }
    else
    {
        QueueEnd++;
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
int freeNode( Node Queue[], Node* Free )
{
    if (QueueLength == 0)
    {
        return 1; //Can't read from queue if empty
    }
    *Free = (Queue[QueueStart]); //Returns the Node
    if (QueueStart == QueueSize-1)
    {
        QueueStart = 0;
    }
    else
    {
        QueueStart++;
    }
    QueueLength--;
    return 0;


}

//======================================================
//=============FORTRESS OF THE LINK MASTER==============
//========http://orteil.dashnet.org/cookieclicker/ =====
//======================================================
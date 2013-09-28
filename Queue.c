#include "functions.h"
#include "Queue.h"


 /********************************************************
 *   Function Name: InitializeQueue(Queue* root)
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
 *   Function Name: addToQueue(Queue* root, int item)
 *
 *   Description: Adds a node to the queue - Pass a node by reference
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
 *   Function Name: popNode(Queue* root, Node* Free)
 *
 *   Description: Pulls the next node off the queue
 *
 *
 *********************************************************/
int popNode( Queue* root, Node* Free )
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

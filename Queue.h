/* 
 * File:   Queue.h
 * Author: Terrial
 *
 * Created on September 23, 2013, 9:35 PM
 */

#ifndef QUEUE_H
#define	QUEUE_H

/*************************************************************************
 Structure Definitions
 ************************************************************************/

typedef struct node
{
    int data;  //Placeholder for actual data
}Node;

typedef struct queue
{
    Node I2CQueue[QueueSize]; //Array of Nodes of the #defined size QueueSize
    uint8 QueueStart; //Location of oldest node
    uint8 QueueEnd; //Location of next free slot
    uint8 QueueLength; //Length of queue
}Queue;

/*************************************************************************
 Variables
 ************************************************************************/
extern Queue I2CQ;

/********************************************************
 *   Function Name: addToQueue(Queue* root, int item)
 *
 *   Description: Adds a node to the queue - Pass a node by reference
 *
 *
 *********************************************************/
int addToQueue( Queue* root, int item );

/********************************************************
 *   Function Name: InitializeQueue(Queue* root)
 *
 *   Description: Clears the queue and resets parameters
 *
 *
 *********************************************************/
void InitializeQueue( Queue* root );

/********************************************************
 *   Function Name: popNode(Queue* root, Node* Free)
 *
 *   Description: Pulls the next node off the queue
 *
 *
 *********************************************************/
int popNode( Queue* root, Node* Free );

#endif	/* QUEUE_H */


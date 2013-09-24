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
    Node I2CQueue[QueueSize];
    uint8 QueueStart;
    uint8 QueueEnd;
    uint8 QueueLength;
}Queue;

/*************************************************************************
 Variables
 ************************************************************************/
extern Queue I2CQ;

/********************************************************
 *   Function Name:
 *
 *   Description:
 *
 *
 *********************************************************/
int addToQueue( Queue* root, int item );

/********************************************************
 *   Function Name:
 *
 *   Description:
 *
 *
 *********************************************************/
void InitializeQueue( Queue* root );

/********************************************************
 *   Function Name:
 *
 *   Description:
 *
 *
 *********************************************************/
int freeNode( Queue* root, Node* Free );

#endif	/* QUEUE_H */


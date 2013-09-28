/*
 * File:   uart.h
 * Author: Ian
 *
 * Created on September 27, 2013, 10:34 AM
 */

#ifndef UART_H
#define	UART_H

/*************************************************************************
 System Includes
 ************************************************************************/
#include <peripheral/ports.h>
#include <peripheral/uart.h>

/*************************************************************************
 Typedefs
 ************************************************************************/

/*************************************************************************
 Constants
 ************************************************************************/
#define UARTQueueSize 16
/*************************************************************************
 Structure Definitions
 ************************************************************************/
typedef struct uart_node
{
    uint8 uart_data[4];
//    uint8 sensor_id;
//    uint8 sensor_sub_id;
//    uint8 data_h;
//    uint8 data_l;
}UART_NODE;

typedef struct uart_queue
{
    UART_NODE DataBank[UARTQueueSize]; //Array of Nodes of the #defined size QueueSize
    uint8 QueueStart; //Location of oldest node
    uint8 QueueEnd; //Location of next free slot
    uint8 QueueLength; //Length of queue
}UART_QUEUE;

/*************************************************************************
 Enums
 ************************************************************************/

/*************************************************************************
 Variables
 ************************************************************************/
UINT8* brecvData;
UINT16 brevSize;

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
void uart_setup(void);
inline void uart_begin(void);

/********************************************************
 *   Function Name: uart_InitializeQueue(UART_QUEUE* queue)
 *
 *   Description: Clears the queue and resets parameters
 *
 *
 *********************************************************/
void uart_InitializeQueue(UART_QUEUE* queue);

/********************************************************
 *   Function Name: uart_addToQueue(UART_QUEUE* queue, UART_NODE new_node)
 *
 *   Description: Adds a node to the queue - Pass a node by reference
 *
 *
 *********************************************************/
int uart_addToQueue(UART_QUEUE* queue, UART_NODE new_node);

/********************************************************
 *   Function Name: uart_popNode(UART_QUEUE* queue, UART_NODE* return_node)
 *
 *   Description: Pulls the next node off the queue
 *
 *
 *********************************************************/
int uart_popNode(UART_QUEUE* queue, UART_NODE* return_node);

 /********************************************************
 *   Function Name: uart_SetNode( uint Byte1, uint Byte2, uint Byte3 )
 *
 *   Description: Creates a node using three bytes
 *
 *
 *********************************************************/
void uart_CreateNode( uint Byte1, uint Byte2, uint Byte3 );

#endif	/* UART_H */


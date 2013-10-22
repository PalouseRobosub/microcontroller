/*
 * File:   uart.h
 * Author: Ian
 *
 * Created on September 27, 2013, 10:34 AM
 */

#ifndef UART1_H
#define	UART1_H

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
#define UART1QueueSize 32
/*************************************************************************
 Structure Definitions
 ************************************************************************/
typedef struct uart1_node
{
    uint8 uart_data[4];
//    uint8 sensor_id;
//    uint8 sensor_sub_id;
//    uint8 data_h;
//    uint8 data_l;
}UART1_NODE;

typedef struct uart1_queue
{
    UART1_NODE DataBank[UART1QueueSize]; //Array of Nodes of the #defined size QueueSize
    uint8 QueueStart; //Location of oldest node
    uint8 QueueEnd; //Location of next free slot
    uint8 QueueLength; //Length of queue
}UART1_QUEUE;

/*************************************************************************
 Enums
 ************************************************************************/

/*************************************************************************
 Variables
 ************************************************************************/

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
void uart1_setup(void);
inline void uart1_begin(void);

/********************************************************
 *   Function Name: uart1_InitializeQueue(UART1_QUEUE* queue)
 *
 *   Description: Clears the queue and resets parameters
 *
 *
 *********************************************************/
void uart1_InitializeQueue(UART1_QUEUE* queue);

/********************************************************
 *   Function Name: uart1_addToQueue(UART1_QUEUE* queue, UART1_NODE new_node)
 *
 *   Description: Adds a node to the queue - Pass a node by reference
 *
 *
 *********************************************************/
int uart1_addToQueue(UART1_QUEUE* queue, UART1_NODE new_node);

/********************************************************
 *   Function Name: uart1_popNode(UART1_QUEUE* queue, UART1_NODE* return_node)
 *
 *   Description: Pulls the next node off the queue
 *
 *
 *********************************************************/
int uart1_popNode(UART1_QUEUE* queue, UART1_NODE* return_node);

 /********************************************************
 *   Function Name: uart1_SetNode( uint Byte1, uint Byte2, uint Byte3 )
 *
 *   Description: Creates a node using three bytes
 *
 *
 *********************************************************/
void uart1_CreateNode( uint Byte1, uint Byte2, uint Byte3 );

#endif	/* UART1_H */


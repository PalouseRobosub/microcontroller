/*
 * File:   motor_UART_ISR.h
 * Author: Korey
 *
 * Created on September 27, 2013, 10:34 AM
 */

#ifndef MOTOR_UART_ISR_H
#define	MOTOR_UART_ISR_H

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
#define MOTOR_UARTQueueSize 32
/*************************************************************************
 Structure Definitions
 ************************************************************************/
typedef struct motor_uart_node
{
    uint8 uart_data[4];
//    uint8 sensor_id;
//    uint8 sensor_sub_id;
//    uint8 data_h;
//    uint8 data_l;
}MOTOR_UART_NODE;

typedef struct motor_uart_queue
{
    MOTOR_UART_NODE DataBank[MOTOR_UARTQueueSize]; //Array of Nodes of the #defined size QueueSize
    uint8 QueueStart; //Location of oldest node
    uint8 QueueEnd; //Location of next free slot
    uint8 QueueLength; //Length of queue
}MOTOR_UART_QUEUE;

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
void motor_uart_setup(void);
inline void motor_uart_begin(void);

/********************************************************
 *   Function Name: motor_uart_InitializeQueue(MOTOR_UART_QUEUE* queue)
 *
 *   Description: Clears the queue and resets parameters
 *
 *
 *********************************************************/
void motor_uart_InitializeQueue(MOTOR_UART_QUEUE* queue);

/********************************************************
 *   Function Name: motor_uart_addToQueue(MOTOR_UART_QUEUE* queue, MOTOR_UART_NODE new_node)
 *
 *   Description: Adds a node to the queue - Pass a node by reference
 *
 *
 *********************************************************/
int motor_uart_addToQueue(MOTOR_UART_QUEUE* queue, MOTOR_UART_NODE new_node);

/********************************************************
 *   Function Name: motor_uart_popNode(MOTOR_UART_QUEUE* queue, MOTOR_UART_NODE* return_node)
 *
 *   Description: Pulls the next node off the queue
 *
 *
 *********************************************************/
int motor_uart_popNode(MOTOR_UART_QUEUE* queue, MOTOR_UART_NODE* return_node);

 /********************************************************
 *   Function Name: motor_uart_SetNode( uint Byte1, uint Byte2, uint Byte3, uint Byte4 )
 *
 *   Description: Creates a node using three bytes
 *
 *
 *********************************************************/
void motor_uart_CreateNode( uint Byte1, uint Byte2, uint Byte3, uint Byte4 );

 /********************************************************
 *   Function Name: Motor1_Forward( uint address, uint speed )
 *
 *   Description: Create a node to move motor 1 forward
 *
 *
 *********************************************************/
void Motor1_Forward( uint address, uint speed );

 /********************************************************
 *   Function Name: Motor1_Backward( uint address, uint speed )
 *
 *   Description: Create a node to move motor 1 backward
 *
 *
 *********************************************************/
void Motor1_Backward( uint address, uint speed );

 /********************************************************
 *   Function Name: Motor1_Forward( uint address, uint speed )
 *
 *   Description: Create a node to move motor 2 forward
 *
 *
 *********************************************************/
void Motor2_Forward( uint address, uint speed );

 /********************************************************
 *   Function Name: Motor1_Backward( uint address, uint speed )
 *
 *   Description: Create a node to move motor 2 backward
 *
 *
 *********************************************************/
void Motor2_Backward( uint address, uint speed );

#endif	/* MOTOR_UART_ISR_H */


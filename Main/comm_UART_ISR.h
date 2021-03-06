/********************************************************
 *   File Name: comm_UART_ISR.h
 *
 *   Description:
 *              header file for the communications UART ISR
 *
 *
 *********************************************************/

#ifndef COMM_UART_ISR_H
#define	COMM_UART_ISR_H

/*************************************************************************
 System Includes
 ************************************************************************/

/*************************************************************************
 Typedefs
 ************************************************************************/

/*************************************************************************
 Constants
 ************************************************************************/
#define COMM_UARTQueueSize 32
#define BG_COMM_UARTQueueSize 64

/*************************************************************************
 Structure Definitions
 ************************************************************************/
typedef struct comm_uart_node {
    uint8 uart_data[4];
    //    uint8 sensor_id;
    //    uint8 sensor_sub_id;
    //    uint8 data_h;
    //    uint8 data_l;
} COMM_UART_NODE;

typedef struct comm_uart_queue {
    COMM_UART_NODE DataBank[COMM_UARTQueueSize]; //Array of Nodes of the #defined size QueueSize
    uint8 QueueStart; //Location of oldest node
    uint8 QueueEnd; //Location of next free slot
    uint8 QueueLength; //Length of queue
} COMM_UART_QUEUE;

typedef struct bg_comm_uart_node {
    uint8 uart_data;
} BG_COMM_UART_NODE;

typedef struct bg_comm_uart_queue {
    BG_COMM_UART_NODE DataBank[BG_COMM_UARTQueueSize]; //Array of Nodes of the #defined size QueueSize
    uint8 QueueStart; //Location of oldest node
    uint8 QueueEnd; //Location of next free slot
    uint8 QueueLength; //Length of queue
} BG_COMM_UART_QUEUE;

typedef struct thruster_status {
    uint8 BOW_SB_MAG;
    boolean BOW_SB_DIR;

    uint8 BOW_PORT_MAG;
    boolean BOW_PORT_DIR;

    uint8 DEPTH_SB_MAG;
    boolean DEPTH_SB_DIR;

    uint8 DEPTH_PORT_MAG;
    boolean DEPTH_PORT_DIR;
    
    uint8 STERN_SB_MAG;
    boolean STERN_SB_DIR;

    uint8 STERN_PORT_MAG;
    boolean STERN_PORT_DIR;
} THRUSTER_STATUS;

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
void comm_uart_setup(void);
inline void comm_uart_begin(void);

/********************************************************
 *   Function Name: comm_uart_InitializeQueue(COMM_UART_QUEUE* queue)
 *
 *   Description: Clears the queue and resets parameters
 *
 *
 *********************************************************/
void comm_uart_InitializeQueue(COMM_UART_QUEUE* queue);

/********************************************************
 *   Function Name: comm_uart_addToQueue(COMM_UART_QUEUE* queue, COMM_UART_NODE new_node)
 *
 *   Description: Adds a node to the queue - Pass a node by reference
 *
 *
 *********************************************************/
int comm_uart_addToQueue(COMM_UART_QUEUE* queue, COMM_UART_NODE new_node);

/********************************************************
 *   Function Name: com_uart_popNode(COMM_UART_QUEUE* queue, COMM_UART_NODE* return_node)
 *
 *   Description: Pulls the next node off the queue
 *
 *
 *********************************************************/
int comm_uart_popNode(COMM_UART_QUEUE* queue, COMM_UART_NODE* return_node);

/********************************************************
 *   Function Name: com_uart_CreateNode( uint Byte1, uint Byte2, uint Byte3 )
 *
 *   Description: Creates a node using three bytes
 *
 *
 *********************************************************/
void comm_uart_CreateNode(uint Byte1, uint Byte2, uint Byte3);



//background queue functions


/********************************************************
 *   Function Name:
 *
 *   Description:
 *
 *
 *********************************************************/
void bg_comm_uart_setup(void);

/********************************************************
 *   Function Name:
 *
 *   Description:
 *
 *
 *********************************************************/
void bg_process_comm_uart(void);

/********************************************************
 *   Function Name:
 *
 *   Description:
 *
 *
 *********************************************************/
void bg_process_thruster_comm_uart(void);

/********************************************************
 *   Function Name:
 *
 *   Description:
 *
 *
 *********************************************************/
void bg_process_actuation_comm_uart(void);

/********************************************************
 *   Function Name: thruster_status_Initialize(THRUSTER_STATUS* thruster_status)
 *
 *   Description: Clears the queue and resets parameters
 *
 *
 *********************************************************/
void thruster_status_Initialize(THRUSTER_STATUS* thruster_status);
/********************************************************
 *   Function Name: bg_comm_uart_InitializeQueue(BG_COMM_UART_QUEUE* queue)
 *
 *   Description: Clears the queue and resets parameters
 *
 *
 *********************************************************/
void bg_comm_uart_InitializeQueue(BG_COMM_UART_QUEUE* queue);

/********************************************************
 *   Function Name: bg_comm_uart_addToQueue(BG_COMM_UART_QUEUE* queue, BG_COMM_UART_NODE new_node)
 *
 *   Description: Adds a node to the queue - Pass a node by reference
 *
 *
 *********************************************************/
int bg_comm_uart_addToQueue(BG_COMM_UART_QUEUE* queue, BG_COMM_UART_NODE new_node);

/********************************************************
 *   Function Name: bg_comm_uart_popNode(BG_COMM_UART_QUEUE* queue, BG_COMM_UART_NODE* return_node)
 *
 *   Description: Pulls the next node off the queue
 *
 *
 *********************************************************/
int bg_comm_uart_popNode(BG_COMM_UART_QUEUE* queue, BG_COMM_UART_NODE* return_node);

/********************************************************
 *   Function Name: bg_comm_uart_CreateNode( uint8 received_byte );
 *
 *   Description: Creates a node using a byte
 *
 *
 *********************************************************/
void bg_comm_uart_CreateNode(uint8 received_byte);

#endif	/* COMM_UART_ISR_H */


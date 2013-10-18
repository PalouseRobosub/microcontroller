/********************************************************
 *   File Name: I2C_ISR.h
 *
 *   Description:
 *              header file for the I2C ISR
 *
 *
 *********************************************************/


#ifndef I2C_ISR_H
#define	I2C_ISR_H

#include "functions.h"
#include "Sensors.h"

/*************************************************************************
 Constants
 ************************************************************************/
#define I2CQueueSize 15
#define I2C_MAX_DATA_SIZE 8
#define I2C_MAX_ADDR_SIZE 2

/*************************************************************************
 Macros
 ************************************************************************/
#define get_read_addr(x) ((x << 1) | 0x01)
#define get_write_addr(x) ((x << 1))

/*************************************************************************
 Enums
 ************************************************************************/

typedef enum //state of the I2C bus
{
    STARTED,
    DEV_ADDR_W_SENT,
    SUB_ADDR_SENT,
    DATA_SENT,
    RESTARTED,
    STOPPED_TEMP,
    STARTED_TEMP,
    DEV_ADDR_R_SENT,
    DATA_RECEIVED,
    ACK_SENT,
    NOACK_SENT,
    STOPPED

} I2C_STATE;

typedef enum    //reading or writing from the sensor
{
    READ,
    WRITE

}I2C_MODE;

/*************************************************************************
 Structure Definitions
 ************************************************************************/


typedef struct i2c_node
{
    SENSOR_ID sensor_id;
    uint8 device_address; //device to read from
    uint8 sub_address[I2C_MAX_ADDR_SIZE]; //sub address(s) to read from
    uint8 sub_address_size; //size (in bytes) of the sub address
    uint8 data_size; //number of bytes expected to write/read
    I2C_MODE mode; //determine if we are reading or writing data
    uint8 tx_data[I2C_MAX_DATA_SIZE]; //data to be written (if using write mode)
}I2C_Node;

typedef struct i2c_queue
{
    I2C_Node DataBank[I2CQueueSize]; //Array of Nodes of the #defined size QueueSize
    uint8 QueueStart; //Location of oldest node
    uint8 QueueEnd; //Location of next free slot
    uint8 QueueLength; //Length of queue
}I2C_Queue;



/*************************************************************************
 Variables
 ************************************************************************/

/*************************************************************************
 Function Declarations
 ************************************************************************/
/********************************************************
 *   Function Name: i2c_1_setup()
 *
 *   Description: Initializes I2C ISR
 *
 *
 *********************************************************/
 void i2c_1_setup(void);
 
 /********************************************************
 *   Function Name: i2c_1_begin()
 *
 *   Description: Starts the I2C state machine
 *
 *
 *********************************************************/
 inline void i2c_1_begin(void);

 /********************************************************
 *   Function Name: I2C_Load_UART(SENSOR_ID sensor, uint8 received_data[])
 *
 *   Description: Packages the received data and puts it
 *                on the UART queue
 *
 *
 *********************************************************/
 void I2C_Load_UART(SENSOR_ID sensor, uint8 received_data[]);

/********************************************************
 *   Function Name: I2C_InitializeQueue(I2C_Queue* queue)
 *
 *   Description: Clears the queue and resets parameters
 *
 *
 *********************************************************/
void I2C_InitializeQueue( I2C_Queue* queue );

/********************************************************
 *   Function Name: I2C_addToQueue(I2C_Queue* queue, I2C_Node new_node)
 *
 *   Description: Adds a node to the queue - Pass a node by reference
 *
 *
 *********************************************************/
int I2C_addToQueue( I2C_Queue* queue, I2C_Node new_node );

/********************************************************
 *   Function Name: I2C_popNode(I2C_Queue* queue, I2C_Node* return_node)
 *
 *   Description: Pulls the next node off the queue
 *
 *
 *********************************************************/
int I2C_popNode( I2C_Queue* queue, I2C_Node* return_node );


#endif	/* I2C_ISR_H */


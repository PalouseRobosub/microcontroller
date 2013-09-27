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

/*************************************************************************
 Constants
 ************************************************************************/
#define I2CQueueSize 15

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
    uint8 device_address; //device to read from
    uint8 sub_address[I2C_MAX_ADDR_SIZE]; //sub address(s) to read from
    uint8 sub_address_size; //size (in bytes) of the sub address
    uint8 data_size; //number of bytes expected to write/read
    I2C_MODE mode; //determine if we are reading or writing data
    uint8 tx_data[I2C_MAX_DATA_SIZE]; //data to be written (if using write mode)
}I2C_Node;

typedef struct i2c_queue
{
    I2C_Node DataBank[I2CQueueSize];
    uint8 QueueStart;
    uint8 QueueEnd;
    uint8 QueueLength;
}I2C_Queue;



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
 void i2c_1_setup(void);

 /********************************************************
 *   Function Name:
 *
 *   Description:
 *
 *
 *********************************************************/
 inline void i2c_1_begin(void);

 /********************************************************
 *   Function Name:
 *
 *   Description:
 *
 *
 *********************************************************/
void I2C_InitializeQueue( I2C_Queue* queue );

 /********************************************************
 *   Function Name:
 *
 *   Description:
 *
 *
 *********************************************************/
int I2C_addToQueue( I2C_Queue* queue, I2C_Node new_node );

/********************************************************
 *   Function Name:
 *
 *   Description:
 *
 *
 *********************************************************/
int I2C_freeNode( I2C_Queue* queue, I2C_Node* return_node );

/********************************************************
 *   Function Name:
 *
 *   Description:
 *
 *
 *********************************************************/
void i2c_ACL(void);

#endif	/* I2C_ISR_H */


/* 
 * File:   I2C.h
 * Author: James Irwin
 *
 * Created on December 22, 2014, 5:22 PM
 */

#ifndef I2C_H
#define	I2C_H

#ifdef	__cplusplus
extern "C" {
#endif


    /*Include Statements*/
#include "System.h"
#include "Queue.h"

#define I2C_SPEED (100000)

    /* Enums */
    typedef enum //reading or writing from the device
    {
        READ,
        WRITE
    } I2C_MODE;

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

    typedef enum {
        I2C1,
        I2C2
    } I2C_Channel;


    /* helpful macros */
#define get_read_addr(x) ((x << 1) | 0x01)
#define get_write_addr(x) ((x << 1))

    /*Object Defintions*/
    //object for references to the data queues

    typedef struct I2C_DATA {
        //Rx queue
        Queue Rx_queue;
        //Tx queue
        Queue Tx_queue;
        //idle information
        boolean Tx_is_idle;
    } I2C_Data;


    //I2C data node

    typedef struct I2C_NODE {
        uint8 device_id; //a unique identifier for the device
        uint8 device_address; //I2C address for device
        uint8 sub_address; //internal device address
        uint8* data_buffer; //data buffer to store the recieved data or write data
        uint8 data_size; //how much data to send/read from device
        //buffer must be large enought able to hold this
        I2C_MODE mode; //reading or writing?
        void *callback; //callback function
    } I2C_Node;



    /*Function Prototypes*/




#ifdef	__cplusplus
}
#endif

#endif	/* I2C_H */

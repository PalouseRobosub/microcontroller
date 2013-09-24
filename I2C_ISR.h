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
#define I2C_CLK_FREQ (100000) //100kHz CLK

/*************************************************************************
 Structure Definitions
 ************************************************************************/


typedef struct i2c_node
{
    uint8 device_address; //device to read from
    uint8 sub_address[I2C_MAX_ADDR_SIZE]; //sub address(s) to read from
    uint8 sub_address_size; //size (in bytes) of the sub address
    boolean write_nread; //determine if we are reading or writing data
    uint8 tx_data[I2C_MAX_DATA_SIZE]; //data to be written (if using write mode)
    uint8 data_size; //number of bytes expected to write/read
   struct i2c_node*   next;
}I2C_Node;

/*************************************************************************
 Enums
 ************************************************************************/
typedef enum //state of the I2C bus
{
    IDLE,
    SENDING_START,
    SELECTING_DEVICE_W,
    WRITING_SUB_ADDR,
    SENDING_RESTART,
    SELECTING_DEVICE_R,
    SENDING_DATA,
    RECEIVING_DATA,
    SENDING_STOP


} I2C_STATE;

/*************************************************************************
 Variables
 ************************************************************************/
extern I2C_STATE i2c_state;


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
void setup_i2c (I2C_MODULE i2c_id);

 /********************************************************
 *   Function Name:
 *
 *   Description:
 *
 *
 *********************************************************/
void i2c_1_isr(void);


#endif	/* I2C_ISR_H */


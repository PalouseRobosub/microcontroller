#include "Sensors.h"
#include "I2C_ISR.h"

/********************************************************
 *   Function Name: i2c_ACL_Read()
 *
 *   Description: Sets the I2C for reading from the PMOD accelerometer
 *
 *
 *********************************************************/
 void i2c_ACL_Read(void)
 {
     I2C_Node temp;
     extern I2C_Queue I2C_1_Queue;

    temp.sensor_id = ACL_0;
    temp.device_address = 0x1D;
    temp.sub_address[0] = 0x32;
    temp.sub_address_size = 1;
    temp.mode = READ;
    temp.data_size = 6;
    temp.tx_data[0] = 0x08;
    I2C_addToQueue(&I2C_1_Queue, temp);
 }

 /********************************************************
 *   Function Name: i2c_ACL_Initialize()
 *
 *   Description: Sets the I2C for writing (Initialize) to the PMOD accelerometer
 *
 *
 *********************************************************/
 void i2c_ACL_Initialize(void)
 {
     I2C_Node temp;
     extern I2C_Queue I2C_1_Queue;

    //data for initializing the PmodALC
    temp.device_address = 0x1D;
    temp.sub_address[0] = 0x2D;
    temp.sub_address_size = 1;
    temp.mode = WRITE;
    temp.data_size = 1;
    temp.tx_data[0] = 0x08;
    I2C_addToQueue(&I2C_1_Queue, temp);
 }

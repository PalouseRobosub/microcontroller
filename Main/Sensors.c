/********************************************************
 *   File Name: Sensors.c
 *
 *   Description:
 *              source code for interacting with various sensors
 *
 *
 *********************************************************/

/*************************************************************************
 System Includes
 ************************************************************************/
#include "system.h"
#if defined (COMPILE_OLD_SUB) || defined (COMPILE_SENSOR_BOARD)

#include "Sensors.h"
#include "I2C_ISR.h"
#include "ADC_ISR.h"

/*************************************************************************
 Variables
 ************************************************************************/


/*************************************************************************
 Functions
 ************************************************************************/


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
     extern I2C_Queue I2C_BANK_0_Queue;

    //data for initializing ACL 0
    temp.device_address = ACL_0_device_address;
    temp.sub_address[0] = ACL_0_WRITE_sub_address;
    temp.sub_address_size = ACL_0_WRITE_address_size;
    temp.mode = WRITE;
    temp.data_size = ACL_0_WRITE_data_size;
    temp.tx_data[0] = ACL_0_WRITE_tx_data;
    I2C_addToQueue(&I2C_BANK_0_Queue, temp);
 }


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
    extern I2C_Queue I2C_BANK_0_Queue;

    //read ACL 0
    temp.sensor_id = ACL_0;
    temp.device_address = ACL_0_device_address;
    temp.sub_address[0] = ACL_0_READ_sub_address;
    temp.sub_address_size = ACL_0_READ_address_size;
    temp.mode = READ;
    temp.data_size = ACL_0_READ_data_size;
    temp.tx_data[0] = ACL_0_READ_tx_data;
    I2C_addToQueue(&I2C_BANK_0_Queue, temp);
 }


  /********************************************************
 *   Function Name: i2c_GYRO_Initialize()
 *
 *   Description: Sets the I2C for writing (Initialize) to the PMOD gyroscope
 *
 *
 *********************************************************/
 void i2c_GYRO_Initialize(void)
 {
     I2C_Node temp;
     extern I2C_Queue I2C_BANK_0_Queue;

    //data for initializing Gyro 0
    temp.device_address = GYRO_0_device_address;
    temp.sub_address[0] = 0x16; //range/filtering control register
    temp.sub_address_size = 1;
    temp.mode = WRITE;
    temp.data_size = 1;
    temp.tx_data[0] = 0x19; //set for 2000 degrees/second range, and LPF BW of 188Hz
    I2C_addToQueue(&I2C_BANK_0_Queue, temp);

 }

 /********************************************************
 *   Function Name: i2c_GYRO_Read()
 *
 *   Description: Sets the I2C for reading from the PMOD gyroscope
 *
 *
 *********************************************************/
 void i2c_GYRO_Read(void)
{
    I2C_Node temp;
    extern I2C_Queue I2C_BANK_0_Queue;

    //read Gyro 0
    temp.sensor_id = GYRO_0;
    temp.device_address = GYRO_0_device_address;
    temp.sub_address[0] = GYRO_0_READ_sub_address; // | (1 << 7); //This device wants MSB to be set when reading multiple bytes
    temp.sub_address_size = GYRO_0_READ_address_size;
    temp.mode = READ;
    temp.data_size = GYRO_0_READ_data_size;
    I2C_addToQueue(&I2C_BANK_0_Queue, temp);

 }

  /********************************************************
 *   Function Name: i2c_MAG_Initialize()
 *
 *   Description: Sets the I2C for writing (Initialize) to the magnometer
 *
 *
 *********************************************************/
 void i2c_MAG_Initialize(void)
 {
     I2C_Node temp;
     extern I2C_Queue I2C_BANK_0_Queue;

    //data for initializing MAG 0
    temp.device_address = MAG_0_device_address;
    temp.sub_address[0] = 0x11;
    temp.sub_address_size = 1;
    temp.mode = WRITE;
    temp.data_size = 1;
    temp.tx_data[0] = 0xA0; //configures for auto reset and raw data mode
    I2C_addToQueue(&I2C_BANK_0_Queue, temp);

    temp.device_address = MAG_0_device_address;
    temp.sub_address[0] = MAG_0_CTRL_REG;
    temp.sub_address_size = 1;
    temp.mode = WRITE;
    temp.data_size = 1;
    temp.tx_data[0] = MAG_0_CTRL_REG_data; //this starts the sensor reading
    I2C_addToQueue(&I2C_BANK_0_Queue, temp);
 }


/********************************************************
 *   Function Name: i2c_MAG_Read()
 *
 *   Description: Sets the I2C for reading from the magnometer
 *
 *
 *********************************************************/
 void i2c_MAG_Read(void)
{
    I2C_Node temp;
    extern I2C_Queue I2C_BANK_0_Queue;

    //read MAG 0
    temp.sensor_id = MAG_0;
    temp.device_address = MAG_0_device_address;
    temp.sub_address[0] = 0x01;
    temp.sub_address_size = 1;
    temp.mode = READ;
    temp.data_size = MAG_0_READ_data_size;
    
    I2C_addToQueue(&I2C_BANK_0_Queue, temp);
 }

 /********************************************************
 *   Function Name: ADC_Depth_Read()
 *
 *   Description: 
 *
 *
 *********************************************************/
 void ADC_Depth_Read(void)
{
    ADC_Node temp;
    extern ADC_Queue ADC_queue;

    temp.sensor_id = ADC_DEPTH;
    temp.adc_channel = ADC_DEPTH_CH;

    ADC_addToQueue(&ADC_queue, temp);

 }

 /********************************************************
 *   Function Name: ADC_Battery_Read()
 *
 *   Description:
 *
 *
 *********************************************************/
 void ADC_Battery_Read(void)
{
    ADC_Node temp;
    extern ADC_Queue ADC_queue;

    temp.sensor_id = ADC_BATT;
    temp.adc_channel = ADC_BATTERY_CH;

    ADC_addToQueue(&ADC_queue, temp);

 }

#endif

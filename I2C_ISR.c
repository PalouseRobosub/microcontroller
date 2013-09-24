/********************************************************
 *   File Name: I2C_ISR.c
 *
 *   Description:
 *              source code file for the I2C ISR
 *
 *
 *********************************************************/
#include "functions.h"
#include "I2C_ISR.h"


/********************************************************
 *   Function Name:
 *
 *   Description:
 *
 *
 *********************************************************/
void setup_i2c (I2C_MODULE i2c_id)
{

    I2CSetFrequency(i2c_id, SYS_CLK_FREQ, I2C_CLK_FREQ);
    I2CEnable(i2c_id, TRUE);

    return;
}

 /********************************************************
 *   Function Name:
 *
 *   Description:
 *
 *
 *********************************************************/
void i2c_1_isr(void)
{
    //variables
    I2C_STATUS current_status;
    static I2C_Node current_sensor;
    uint8 received_data[I2C_MAX_DATA_SIZE];
    static uint8 sub_address_index;
    static uint8 data_index;

    //disable interrupts
    //<code to disable interrupts>

    //clear interrupt flag
    //<code to clear interrupt flag>

    //get I2C status
    current_status = I2CGetStatus(I2C1);


    switch (i2c_state)
    {
        case IDLE:
            break;


        case SENDING_START:
            //current_sensor = <dequeue node function>
                if (current_sensor.device_address == 0)  //error, queue is empty!
                {
                    return;
                }
            I2CStart(I2C1);
            i2c_state = SELECTING_DEVICE_W;
            break;


        case SELECTING_DEVICE_W:
            //confirm start signal was sent
            if ((current_status & I2C_START) == I2C_START)
            {
                //shifting the address left and adding the write bit
                I2CSendByte(I2C1, ((current_sensor.device_address << 1) & 0xFE));
                i2c_state = WRITING_SUB_ADDR;
            }
            else
            {
                //error, start signal was not sent
            }
            break;
        case WRITING_SUB_ADDR:
            //confirm the device acknowledged the hail
            if ((current_status & I2C_BYTE_ACKNOWLEDGED) == I2C_BYTE_ACKNOWLEDGED)
            {
                //send the sub address
                I2CSendByte(I2C1, current_sensor.sub_address[sub_address_index]);

                //increment the sub address index and check if we have
                //written the full address
                ++sub_address_index;
                if (sub_address_index == current_sensor.sub_address_size)
                {
                    if (current_sensor.write_nread == TRUE) //writing data
                    {
                        i2c_state = SENDING_DATA;

                    }
                    else //reading_data
                    {
                        i2c_state = SENDING_RESTART;
                    }
                }
            }
            else
            {
                //error, device did not acknowledge the hail
            }
            break;
        case SENDING_RESTART:
             //confirm the device acknowledged sub address
            if ((current_status & I2C_BYTE_ACKNOWLEDGED) == I2C_BYTE_ACKNOWLEDGED)
            {
                I2CRepeatStart(I2C1);
                i2c_state = SELECTING_DEVICE_R;
            }
            else
            {
                //error, device did not acknowledge the sub address
            }
            break;
        case SELECTING_DEVICE_R:
             //confirm restart signal was sent
            if ((current_status & I2C_START) == I2C_START)
            {
                //shifting the address left and adding the write bit
                I2CSendByte(I2C1, ((current_sensor.device_address << 1) | 0x01));
                i2c_state = RECEIVING_DATA;
            }
            else
            {
                //error, restart signal was not sent
            }
            break;
        case RECEIVING_DATA:
            //confirm the device acknowledged the hail
            if ((current_status & I2C_BYTE_ACKNOWLEDGED) == I2C_BYTE_ACKNOWLEDGED)
            {
                received_data[data_index] = I2CGetByte(I2C1);

                ++data_index;
                if (data_index == current_sensor.data_size)
                {
                 i2c_state = SENDING_STOP;
                }

            }
            else
            {
                //error, device did not acknowledge the hail
            }
            break;
        case SENDING_DATA:
            //confirm the device acknowledged the sent data
            if ((current_status & I2C_BYTE_ACKNOWLEDGED) == I2C_BYTE_ACKNOWLEDGED)
            {
                I2CSendByte(I2C1, current_sensor.tx_data[data_index]);

                ++data_index;
                if (data_index == current_sensor.data_size)
                {
                 i2c_state = SENDING_STOP;
                }

            }
            else
            {
                //error, device did not acknowledge the sent data
            }
            break;
        case SENDING_STOP:
            I2CStop(I2C1);
            i2c_state = IDLE;
            break;
        default:
            //error
            break;
    }



    //re-enable interrupts
    return;
}
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


/*************************************************************************
 Variables
 ************************************************************************/
I2C_STATE state;
uint8 data;

/********************************************************
 *   Function Name:
 *
 *   Description:
 *
 *
 *********************************************************/
 void i2c_1_setup(void)
 {
     //actualClock = I2CSetFrequency(I2C_BUS, SYS_FREQ, I2C_FREQ);
    I2C1BRG = 0x2F;

    //I2CEnable(I2C_BUS, TRUE);
    I2C1CONbits.ON = 1;

    //Setup I2C interrupts
    IEC0SET = (1 << 31); //enable interrupt
    IPC6SET = (2 << 10); //set priority
 }

 inline void i2c_1_begin(void)
 {
     I2C1CONbits.SEN = 1; //start the I2C transaction
     state = STARTED;
 }

 void __ISR(_I2C_1_VECTOR, ipl2) _I2C_1_Handler(void)
 {
    static I2C_Node current_node;
    static uint8 received_data[I2C_MAX_DATA_SIZE];
    static uint8 sub_address_index;
    static uint8 data_index;

    current_node.device_address = 0x1D;
    current_node.sub_address[0] = 0x00;
    current_node.sub_address_size = 1;
    current_node.mode = READ;
    current_node.data_size = 2;

 IFS0bits.I2C1MIF = 0; //clear the interrupt
 //PORTGbits.RG1 = !PORTGbits.RG1; //toggle pin

 switch(state)
 {
     case STARTED:
         I2C1TRN = get_write_addr(current_node.device_address);
         sub_address_index = 0;
         data_index = 0;
         state = DEV_ADDR_W_SENT;
         break;

     case DEV_ADDR_W_SENT:
         I2C1TRN = current_node.sub_address[sub_address_index];
         
         ++sub_address_index;
         if (sub_address_index == current_node.sub_address_size)
         {
            state = SUB_ADDR_SENT;
         }
         break;

     case SUB_ADDR_SENT:
         if (current_node.mode == READ)
         {
         I2C1CONbits.RSEN = 1;
         state = RESTARTED;
         }
         //else
         break;

     case RESTARTED:
         I2C1TRN = get_read_addr(current_node.device_address);
         state = DEV_ADDR_R_SENT;
         break;

     case DEV_ADDR_R_SENT:
         I2C1CONbits.RCEN = 1;
         state = DATA_RECEIVED;
         break;

     case DATA_RECEIVED:
         received_data[data_index] = I2C1RCV;

        ++data_index;
        if (data_index == current_node.data_size)
        {
            I2C1CONbits.ACKDT = 1;
            state = NOACK_SENT;
        }
        else
        {
            I2C1CONbits.ACKDT = 0;
            state = ACK_SENT;
        }

        I2C1CONbits.ACKEN = 1;

        break;

     case ACK_SENT:
         I2C1CONbits.RCEN = 1;
         state = DATA_RECEIVED;
         break;

     case NOACK_SENT:
         I2C1CONbits.PEN = 1;
         state = STOPPED;
         break;

     case STOPPED:
         delay();
         I2C1CONbits.SEN = 1;
         state = STARTED;
         break;
 }

 return;
 }

 /********************************************************
 *   Function Name:
 *
 *   Description:
 *
 *
 *********************************************************/
//void i2c_1_isr(void)
//{
//    //variables
//    I2C_STATUS current_status;
//    static I2C_Node current_sensor;
//    uint8 received_data[I2C_MAX_DATA_SIZE];
//    static uint8 sub_address_index;
//    static uint8 data_index;
//
//    current_sensor.device_address = 0X17;
//
//    //disable interrupts
//    //<code to disable interrupts>
//
//    //clear interrupt flag
//    //<code to clear interrupt flag>
//
//    //get I2C status
//    current_status = I2CGetStatus(I2C1);
//
//
//    switch (i2c_state)
//    {
//        case IDLE:
//            break;
//
//
//        case SENDING_START:
//            //current_sensor = <dequeue node function>
//                if (current_sensor.device_address == 0)  //error, queue is empty!
//                {
//                    return;
//                }
//            I2CStart(I2C1);
//            i2c_state = SELECTING_DEVICE_W;
//            break;
//
//
//        case SELECTING_DEVICE_W:
//            //confirm start signal was sent
//            if ((current_status & I2C_START) == I2C_START)
//            {
//                //shifting the address left and adding the write bit
//                I2CSendByte(I2C1, ((current_sensor.device_address << 1) & 0xFE));
//                i2c_state = WRITING_SUB_ADDR;
//            }
//            else
//            {
//                //error, start signal was not sent
//            }
//            break;
//        case WRITING_SUB_ADDR:
//            //confirm the device acknowledged the hail
//            if ((current_status & I2C_BYTE_ACKNOWLEDGED) == I2C_BYTE_ACKNOWLEDGED)
//            {
//                //send the sub address
//                I2CSendByte(I2C1, current_sensor.sub_address[sub_address_index]);
//
//                //increment the sub address index and check if we have
//                //written the full address
//                ++sub_address_index;
//                if (sub_address_index == current_sensor.sub_address_size)
//                {
//                    if (current_sensor.write_nread == TRUE) //writing data
//                    {
//                        i2c_state = SENDING_DATA;
//
//                    }
//                    else //reading_data
//                    {
//                        i2c_state = SENDING_RESTART;
//                    }
//                }
//            }
//            else
//            {
//                //error, device did not acknowledge the hail
//            }
//            break;
//        case SENDING_RESTART:
//             //confirm the device acknowledged sub address
//            if ((current_status & I2C_BYTE_ACKNOWLEDGED) == I2C_BYTE_ACKNOWLEDGED)
//            {
//                I2CRepeatStart(I2C1);
//                i2c_state = SELECTING_DEVICE_R;
//            }
//            else
//            {
//                //error, device did not acknowledge the sub address
//            }
//            break;
//        case SELECTING_DEVICE_R:
//             //confirm restart signal was sent
//            if ((current_status & I2C_START) == I2C_START)
//            {
//                //shifting the address left and adding the write bit
//                I2CSendByte(I2C1, ((current_sensor.device_address << 1) | 0x01));
//                i2c_state = RECEIVING_DATA;
//            }
//            else
//            {
//                //error, restart signal was not sent
//            }
//            break;
//        case RECEIVING_DATA:
//            //confirm the device acknowledged the hail
//            if ((current_status & I2C_BYTE_ACKNOWLEDGED) == I2C_BYTE_ACKNOWLEDGED)
//            {
//                received_data[data_index] = I2CGetByte(I2C1);
//
//                ++data_index;
//                if (data_index == current_sensor.data_size)
//                {
//                 i2c_state = SENDING_STOP;
//                }
//
//            }
//            else
//            {
//                //error, device did not acknowledge the hail
//            }
//            break;
//        case SENDING_DATA:
//            //confirm the device acknowledged the sent data
//            if ((current_status & I2C_BYTE_ACKNOWLEDGED) == I2C_BYTE_ACKNOWLEDGED)
//            {
//                I2CSendByte(I2C1, current_sensor.tx_data[data_index]);
//
//                ++data_index;
//                if (data_index == current_sensor.data_size)
//                {
//                 i2c_state = SENDING_STOP;
//                }
//
//            }
//            else
//            {
//                //error, device did not acknowledge the sent data
//            }
//            break;
//        case SENDING_STOP:
//            I2CStop(I2C1);
//            i2c_state = IDLE;
//            break;
//        default:
//            //error
//            break;
//    }
//
//
//
//    //re-enable interrupts
//    return;
//}
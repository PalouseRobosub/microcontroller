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
I2C_Queue I2C_1_Queue;
/********************************************************
 *   Function Name:
 *
 *   Description:
 *
 *
 *********************************************************/
 void i2c_1_setup(void)
 {
     I2C_Node temp;
     //actualClock = I2CSetFrequency(I2C_BUS, SYS_FREQ, I2C_FREQ);
    I2C1BRG = 0x2F;

    //I2CEnable(I2C_BUS, TRUE);
    I2C1CONbits.ON = 1;

    //Setup I2C interrupts
    IEC0SET = (1 << 31); //enable interrupt
    IPC6SET = (2 << 10); //set priority

    I2C_InitializeQueue(&I2C_1_Queue); //initialize the queue

    //data for initializing the PmodALC
    temp.device_address = 0x1D;
    temp.sub_address[0] = 0x2D;
    temp.sub_address_size = 1;
    temp.mode = WRITE;
    temp.data_size = 1;
    temp.tx_data[0] = 0x08;
    I2C_addToQueue(&I2C_1_Queue, temp);
    temp.device_address = 0x1D;
    temp.sub_address[0] = 0x32;
    temp.sub_address_size = 1;
    temp.mode = READ;
    temp.data_size = 6;
    temp.tx_data[0] = 0x08;
    I2C_addToQueue(&I2C_1_Queue, temp);
 }

 inline void i2c_1_begin(void)
 {
    // I2C1CONbits.SEN = 1; //start the I2C transaction
     state = STOPPED;
     IFS0bits.I2C1MIF = 1;
 }

 void __ISR(_I2C_1_VECTOR, ipl2) _I2C_1_Handler(void)
 {
    static I2C_Node current_node;
    static uint8 received_data[I2C_MAX_DATA_SIZE];
    static uint8 sub_address_index;
    static uint8 data_index;

//    current_node.device_address = 0x1D;
//    current_node.sub_address[0] = 0x32;
//    current_node.sub_address_size = 1;
//    current_node.mode = READ;
//    current_node.data_size = 1;
//    current_node.tx_data[0] = 0x08;

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
         else
         {
             I2C1TRN = current_node.tx_data[data_index];
             state = DATA_SENT;
         }
         break;

     case DATA_SENT:
         ++data_index;

         if (data_index == current_node.data_size)
         {
             I2C1CONbits.PEN = 1;
             state = STOPPED;
         }
         else
         {
             I2C1TRN = current_node.tx_data[data_index];           
         }
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
         I2C_freeNode(&I2C_1_Queue, &current_node);
         I2C1CONbits.SEN = 1;
         state = STARTED;
         break;
 }

 return;
 }

 /********************************************************
 *   Function Name: I2C_InitializeQueue()
 *
 *   Description: Clears the queue and resets parameters
 *
 *
 *********************************************************/
void I2C_InitializeQueue( I2C_Queue* queue )
{
    memset(queue, 0, sizeof(I2C_Queue));
}
 /********************************************************
 *   Function Name:
 *
 *   Description:
 *
 *
 *********************************************************/
int I2C_addToQueue( I2C_Queue* queue, I2C_Node new_node )
{
    if (queue->QueueEnd == queue->QueueStart && queue->QueueLength > 0)
    {
        return 1; //Error, would overwrite start of list
    }
    queue->DataBank[queue->QueueEnd] = new_node;
    queue->QueueLength++;
    if (queue->QueueEnd == I2CQueueSize-1)
    {
        queue->QueueEnd = 0;
    }
    else
    {
        queue->QueueEnd++;
    }
    return 0;
}
  /********************************************************
 *   Function Name:
 *
 *   Description:
 *
 *
 *********************************************************/
int I2C_freeNode( I2C_Queue* queue, I2C_Node* return_node )
{
    if (queue->QueueLength == 0)
    {
        return 1; //Can't read from queue if empty
    }
    *return_node = (queue->DataBank[queue->QueueStart]); //Returns the Node
    if (queue->QueueStart == I2CQueueSize-1)
    {
        queue->QueueStart = 0;
    }
    else
    {
        queue->QueueStart++;
    }
    queue->QueueLength--;
    return 0;


}
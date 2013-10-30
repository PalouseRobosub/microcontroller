/********************************************************
 *   File Name: I2C_BANK_0_ISR.c
 *
 *   Description:
 *              source code file for the I2C BANK 0 ISR
 *
 *
 *********************************************************/
#include "system.h"
#include "functions.h"
#include "Sensors.h"
#include "I2C_ISR.h"


/*************************************************************************
 Variables
 ************************************************************************/
I2C_STATE state;
I2C_Queue I2C_BANK_0_Queue;
boolean I2C_BANK_0_is_idle;

/********************************************************
 *   Function Name: i2c_bank_0_setup()
 *
 *   Description: Initializes I2C ISR
 *
 *
 *********************************************************/
 void i2c_bank_0_setup(void)
 {
     
    //configure the clock for the I2C1
    I2C_BANK_0_BRG = 0x2F; //if main clock is 80 MHz, use 0x2F for 100kHz I2C

    //Setup I2C1 interrupts
    I2C_BANK_0_INT_PRIORITY_set(7); //set priority
    I2C_BANK_0_INT_set(1); //enable interrupt

    //enable the I2C1 module
    I2C_BANK_0_ON = 1;

    I2C_InitializeQueue(&I2C_BANK_0_Queue); //initialize the queue

    I2C_BANK_0_is_idle = TRUE; //set that bus is currently idle
 }

 /********************************************************
 *   Function Name: i2c_bank_0_begin()
 *
 *   Description: Starts the I2C state machine
 *
 *
 *********************************************************/
 inline void i2c_bank_0_begin(void)
 {
     state = STOPPED; //initialize state machine

     /* this sets the I2C1 interrupt flag. Setting the flag will
     cause the ISR to be entered as soon as the global interrupt
     flag is enabled */
     I2C_BANK_0_MIF = 1;
 }


 /********************************************************
 *   Function Name: _I2C_1_Handler
 *
 *   Description:
 *          ISR for the I2C_BANK_0 module
 *
 *********************************************************/
 void __ISR(_I2C_BANK_0_VECTOR, IPL7AUTO) I2C_BANK_0_Handler(void)
 {
    static I2C_Node current_node;
    static uint8 received_data[I2C_MAX_DATA_SIZE];
    static uint8 sub_address_index;
    static uint8 data_index;
    extern boolean COMM_UART_is_idle;

    INTDisableInterrupts();
    I2C_BANK_0_MIF = 0; //clear the interrupt flag
    
    //PORTGbits.RG0 = !PORTGbits.RG0; //for testing, remove in final code

 switch(state)
 {
     case STARTED: //start signal has been sent, now to send the device addresss + write bit
         I2C_BANK_0_TRN = get_write_addr(current_node.device_address); //send device address + write bit
         
         //initialize indexes
         sub_address_index = 0;
         data_index = 0;
         //move on to next state
         state = DEV_ADDR_W_SENT;
         break;

     case DEV_ADDR_W_SENT: //device addresss + write bit has been sent, now to send the sub address
         I2C_BANK_0_TRN = current_node.sub_address[sub_address_index]; //send first sub address byte
         //logic for sending more than one sub address byte
         ++sub_address_index;
         if (sub_address_index == current_node.sub_address_size)
         {
            state = SUB_ADDR_SENT; //we have sent all sub address bytes, move to next state
         }
         break;

     case SUB_ADDR_SENT: //sub address has been sent, need logic to determine next step
         if (current_node.mode == READ) //if we want to read
         {
         I2C_BANK_0_RSEN = 1; //send restart signal
         state = RESTARTED; //move onto next state
         }
         else   //if we want to write
         {
             I2C_BANK_0_TRN = current_node.tx_data[data_index]; //send first data byte
             state = DATA_SENT; //move on to next state
         }
         break;

     case DATA_SENT: //we have just sent a data byte
         ++data_index;

         if (data_index == current_node.data_size) //if we have sent all data bytes
         {
             I2C_BANK_0_PEN = 1;   //send the stop signal
             state = STOPPED;       //move onto next state
         }
         else //if we have more bytes to send
         {
             I2C_BANK_0_TRN = current_node.tx_data[data_index]; //start sending next bit
         }
         break;

     case RESTARTED: //restart signal has been sent, now to send device address + read bit
         I2C_BANK_0_TRN = get_read_addr(current_node.device_address); //send device address + read bit
         state = DEV_ADDR_R_SENT; //move onto next state
         break;

     case DEV_ADDR_R_SENT: //device address + read bit has been sent, get ready to receive byte(s)
         I2C_BANK_0_RCEN = 1;  //enable receiver mode
         state = DATA_RECEIVED; //move onto next state
         break;

     case DATA_RECEIVED: //we have just received a byte
         received_data[data_index] = I2C_BANK_0_RCV; //read the received data

        ++data_index;
        if (data_index == current_node.data_size) //if we have read all the data we want
        {
            I2C_BANK_0_ACKDT = 1;  //configure to send NACK signal
            state = NOACK_SENT; //move onto next state
        }
        else    //we want to read another byte
        {
            I2C_BANK_0_ACKDT = 0; //configure to send ACK signal
            state = ACK_SENT;      //move onto next state
        }

        I2C_BANK_0_ACKEN = 1; //send the ACK/NACK signal

        break;

     case ACK_SENT: //we have sent an ACK signal, prepare to receive another byte
         I2C_BANK_0_RCEN = 1;  //enable the receiver mode
         state = DATA_RECEIVED; //move onto next state
         break;

     case NOACK_SENT:   //we have sent an NACK signal, prepare to stop transaction
         I2C_BANK_0_PEN = 1; //send the stop signal

         //create uart node
         I2C_Load_UART(current_node.sensor_id, received_data);
         
         if (COMM_UART_is_idle)
         {
            comm_uart_begin();
         }

         state = STOPPED; //move onto next state
         break;

     case STOPPED:  //we have just sent the stop signal
         //delay(); //this is for testing (spaces out the I2C transactions). Remove in final code!
         if (I2C_popNode(&I2C_BANK_0_Queue, &current_node)) //load next node from the queue
         {
             I2C_BANK_0_is_idle = TRUE; //flag that the bus is idle (nothing in the send queue)
         }
         else
         {
            I2C_BANK_0_SEN = 1; //send the start signal
            I2C_BANK_0_is_idle = FALSE; //flag that the bus is working now
            state = STARTED; //move onto next state
         }
         break;
 }


INTEnableInterrupts();
 return;
 }

/********************************************************
 *   Function Name: I2C_Load_UART(SENSOR_ID sensor, uint8 received_data[])
 *
 *   Description: Packages the received data and puts it
 *                on the UART queue
 *
 *
 *********************************************************/
void I2C_Load_UART(SENSOR_ID sensor, uint8 received_data[])
{

    switch (sensor)
    {
        case ACL_0:
            comm_uart_CreateNode(ACL_0_X, received_data[0], received_data[1]);
            comm_uart_CreateNode(ACL_0_Y, received_data[2], received_data[3]);
            comm_uart_CreateNode(ACL_0_Z, received_data[4], received_data[5]);
            break;

        case GYRO_0:
            comm_uart_CreateNode(GYRO_0_X, received_data[0], received_data[1]);
            comm_uart_CreateNode(GYRO_0_Y, received_data[2], received_data[3]);
            comm_uart_CreateNode(GYRO_0_Z, received_data[4], received_data[5]);
            break;
    }

    return;
}

/********************************************************
 *   Function Name: I2C_InitializeQueue(I2C_Queue* queue)
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
 *   Function Name: I2C_addToQueue(I2C_Queue* queue, I2C_Node new_node)
 *
 *   Description: Adds a node to the queue - Pass a node by reference
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
 *   Function Name: I2C_popNode(I2C_Queue* queue, I2C_Node* return_node)
 *
 *   Description: Pulls the next node off the queue
 *
 *
 *********************************************************/
int I2C_popNode( I2C_Queue* queue, I2C_Node* return_node )
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
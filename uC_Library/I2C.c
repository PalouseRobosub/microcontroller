
#include "I2C.h"

//callback functions
void (*I2C_1_callback) (void);
void (*I2C_2_callback) (void);

//channel data structures
I2C_Data i2c1;
I2C_Data i2c2;

//i2c state machine variables
I2C_STATE i2c_1_state;
I2C_STATE i2c_2_state;

I2C_Data* initialize_I2C(uint pb_clk, I2C_Channel channel, uint8 *rx_buffer_ptr, uint8 rx_buffer_size,
        uint8 *tx_buffer_ptr, uint8 tx_buffer_size, void* callback) {

    switch (channel) {
        case I2C1:
            I2C1BRG = 1/(2*I2C_SPEED)*pb_clk - 2;  //calculate the proper divider

            //setup the rx and tx buffers
            i2c1.Rx_queue = create_queue(rx_buffer_ptr, rx_buffer_size);
            i2c1.Tx_queue = create_queue(tx_buffer_ptr, tx_buffer_size);

            I2C_1_callback = callback; //link the callback function

            i2c1.Tx_is_idle = TRUE;

            I2C1CONbits.ON = 0b1; //enable the I2C module'
            return &i2c1;
            break;

        default:
            //some sort of error handling?
            break;
    }

    return NULL;
}

void __ISR(_I2C_1_VECTOR, IPL7AUTO) I2C_1_Handler(void) {
    static I2C_Node current_node;
    static uint8* data;
    static uint8 sub_address_index;
    static uint8 data_index;
    extern boolean COMM_UART_is_idle;

    asm volatile ("di"); //disable interrupts

    IFS1bits.I2C1MIF = 0; //clear the interrupt flag


 switch(i2c_1_state)
 {
     case STARTED: //start signal has been sent, now to send the device addresss + write bit
         I2C1TRN = get_write_addr(current_node.device_address); //send device address + write bit

         //initialize indexes
         sub_address_index = 0;
         data_index = 0;
         //move on to next state
         i2c_1_state = DEV_ADDR_W_SENT;
         break;

     case DEV_ADDR_W_SENT: //device addresss + write bit has been sent, now to send the sub address
         I2C1TRN = current_node.sub_address[sub_address_index]; //send first sub address byte
         //logic for sending more than one sub address byte
         ++sub_address_index;
         if (sub_address_index == current_node.sub_address_size)
         {
            i2c_1_state = SUB_ADDR_SENT; //we have sent all sub address bytes, move to next state
         }
         break;

     case SUB_ADDR_SENT: //sub address has been sent, need logic to determine next step
         if (current_node.mode == READ) //if we want to read
         {
         I2C1CONbits.RSEN = 1; //send restart signal
         i2c_1_state = RESTARTED; //move onto next state
         }
         else   //if we want to write
         {
             I2C1TRN = current_node.tx_data[data_index]; //send first data byte
             i2c_1_state = DATA_SENT; //move on to next state
         }
         break;

     case DATA_SENT: //we have just sent a data byte
         ++data_index;

         if (data_index == current_node.data_size) //if we have sent all data bytes
         {
             I2C1CONbits.PEN = 1;   //send the stop signal
             i2c_1_state = STOPPED;       //move onto next state
         }
         else //if we have more bytes to send
         {
             I2C1TRN = current_node.tx_data[data_index]; //start sending next bit
         }
         break;

     case RESTARTED: //restart signal has been sent, now to send device address + read bit
         I2C1TRN = get_read_addr(current_node.device_address); //send device address + read bit
         i2c_1_state = DEV_ADDR_R_SENT; //move onto next state
         break;

     case DEV_ADDR_R_SENT: //device address + read bit has been sent, get ready to receive byte(s)
         I2C1CONbits.RCEN = 1;  //enable receiver mode
         i2c_1_state = DATA_RECEIVED; //move onto next state
         break;

     case DATA_RECEIVED: //we have just received a byte
         received_data[data_index] = I2C1RCV; //read the received data

        ++data_index;
        if (data_index == current_node.data_size) //if we have read all the data we want
        {
            I2C1CONbits.ACKDT = 1;  //configure to send NACK signal
            i2c_1_state = NOACK_SENT; //move onto next state
        }
        else    //we want to read another byte
        {
            I2C1CONbits.ACKDT = 0; //configure to send ACK signal
            i2c_1_state = ACK_SENT;      //move onto next state
        }

        I2C1CONbits.ACKEN = 1; //send the ACK/NACK signal

        break;

     case ACK_SENT: //we have sent an ACK signal, prepare to receive another byte
         I2C1CONbits.RCEN = 1;  //enable the receiver mode
         i2c_1_state = DATA_RECEIVED; //move onto next state
         break;

     case NOACK_SENT:   //we have sent an NACK signal, prepare to stop transaction
         I2C1CONbits.PEN = 1; //send the stop signal

         //create uart node
         I2C_Load_UART(current_node.sensor_id, received_data);

         if (COMM_UART_is_idle)
         {
            comm_uart_begin();
         }

         i2c_1_state = STOPPED; //move onto next state
         break;

     case STOPPED:  //we have just sent the stop signal
         if (I2C_popNode(&I2C_BANK_0_Queue, &current_node)) //load next node from the queue
         {
             i2c1.Tx_is_idle = TRUE; //flag that the bus is idle (nothing in the send queue)
         }
         else
         {
            I2C1CONbits.SEN = 1; //send the start signal
            i2c1.Tx_is_idle = FALSE; //flag that the bus is working now
            i2c_1_state = STARTED; //move onto next state
         }
         break;
 }

    asm volatile ("ei"); //reenable interrupts
}

void __ISR(_I2C_2_VECTOR, IPL7AUTO) I2C_2_Handler(void) {
    asm volatile ("di"); //disable interrupt


    asm volatile ("ei"); //reenable interrupts
}

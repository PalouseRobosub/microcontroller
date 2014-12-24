
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
            I2C1BRG = 1 / (2 * I2C_SPEED) * pb_clk - 2; //calculate the proper divider

            //setup the rx and tx buffers
            i2c1.Rx_queue = create_queue(rx_buffer_ptr, rx_buffer_size);
            i2c1.Tx_queue = create_queue(tx_buffer_ptr, tx_buffer_size);

            I2C_1_callback = callback; //link the callback function

            i2c1.Tx_is_idle = TRUE; //set the I2C state machine to idling
            i2c_1_state = STOPPED;

            I2C1CONbits.ON = 0b1; //enable the I2C module
            return &i2c1;
            break;

        case I2C2:
            I2C2BRG = 1 / (2 * I2C_SPEED) * pb_clk - 2; //calculate the proper divider

            //setup the rx and tx buffers
            i2c2.Rx_queue = create_queue(rx_buffer_ptr, rx_buffer_size);
            i2c2.Tx_queue = create_queue(tx_buffer_ptr, tx_buffer_size);

            I2C_2_callback = callback; //link the callback function

            i2c2.Tx_is_idle = TRUE; //set the I2C state machine to idling
            i2c_2_state = STOPPED;

            I2C2CONbits.ON = 0b1; //enable the I2C module
            return &i2c2;
            break;

        default:
            //some sort of error handling?
            break;
    }

    return NULL;
}

int send_I2C(I2C_Channel channel, uint8 device_id, uint8 device_address,
             uint8 sub_address, uint8* data_buffer, uint8 data_size,
             I2C_MODE read_write, void* callback) {
    I2C_Node new_node;
    int status;

    new_node.device_id = device_id;
    new_node.device_address = device_address;
    new_node.sub_address = sub_address;
    new_node.data_buffer = data_buffer;
    new_node.data_size = data_size;
    new_node.mode = read_write;
    new_node.callback = callback;

    switch (channel) {
        case I2C1:
            //load the new node
            status = enqueue(&(i2c1.Tx_queue), (uint8*) & new_node, sizeof (new_node));

            //if the bus is idling, force-start it
            if (i2c1.Tx_is_idle) {
                IFS1bits.I2C1MIF = 1;
            }
            break;

        case I2C2:
            //load the new node
            status = enqueue(&(i2c2.Tx_queue), (uint8*) & new_node, sizeof (new_node));

            //if the bus is idling, force-start it
            if (i2c2.Tx_is_idle) {
                IFS1bits.I2C2MIF = 1;
            }

            break;

        default:
            //some sort of error handling?
            status = 1;
    }

    return status;
}

//run this background process in the main while loop to
//process the results of I2C transactions

int bg_process_I2C(void) {
    I2C_Node current_node;

    //process channel 1
    while (!dequeue(&(i2c1.Tx_queue), (uint8*) & current_node, sizeof (current_node))) {
        if (current_node.callback != NULL) {
            current_node.callback(current_node);
        }
    }


    //process channel 2
    while (!dequeue(&(i2c2.Tx_queue), (uint8*) & current_node, sizeof (current_node))) {
        if (current_node.callback != NULL) {
            current_node.callback(current_node);
        }
    }
}

void __ISR(_I2C_1_VECTOR, IPL7AUTO) I2C_1_Handler(void) {
    static I2C_Node current_node;
    static uint8 data_index;

    asm volatile ("di"); //disable interrupts

    IFS1bits.I2C1MIF = 0; //clear the interrupt flag


    switch (i2c_1_state) {
        case STARTED: //start signal has been sent, now to send the device addresss + write bit
            I2C1TRN = get_write_addr(current_node.device_address); //send device address + write bit

            //initialize indexes
            data_index = 0;
            //move on to next state
            i2c_1_state = DEV_ADDR_W_SENT;
            break;

        case DEV_ADDR_W_SENT: //device addresss + write bit has been sent, now to send the sub address
            I2C1TRN = current_node.sub_address; //send sub address byte
            i2c_1_state = SUB_ADDR_SENT; //move to next state
            break;

        case SUB_ADDR_SENT: //sub address has been sent, need logic to determine next step
            if (current_node.mode == READ) //if we want to read
            {
                I2C1CONbits.RSEN = 1; //send restart signal
                i2c_1_state = RESTARTED; //move onto next state
            } else //if we want to write
            {
                I2C1TRN = current_node.data_buffer[data_index]; //send first data byte
                i2c_1_state = DATA_SENT; //move on to next state
            }
            break;

        case DATA_SENT: //we have just sent a data byte
            ++data_index;

            if (data_index == current_node.data_size) //if we have sent all data bytes
            {
                I2C1CONbits.PEN = 1; //send the stop signal
                i2c_1_state = STOPPED; //move onto next state
            } else //if we have more bytes to send
            {
                I2C1TRN = current_node.data_buffer[data_index]; //start sending next bit
            }
            break;

        case RESTARTED: //restart signal has been sent, now to send device address + read bit
            I2C1TRN = get_read_addr(current_node.device_address); //send device address + read bit
            i2c_1_state = DEV_ADDR_R_SENT; //move onto next state
            break;

        case DEV_ADDR_R_SENT: //device address + read bit has been sent, get ready to receive byte(s)
            I2C1CONbits.RCEN = 1; //enable receiver mode
            i2c_1_state = DATA_RECEIVED; //move onto next state
            break;

        case DATA_RECEIVED: //we have just received a byte
            current_node.data_buffer[data_index] = I2C1RCV; //read the received data

            ++data_index;
            if (data_index == current_node.data_size) //if we have read all the data we want
            {
                I2C1CONbits.ACKDT = 1; //configure to send NACK signal
                i2c_1_state = NOACK_SENT; //move onto next state
            } else //we want to read another byte
            {
                I2C1CONbits.ACKDT = 0; //configure to send ACK signal
                i2c_1_state = ACK_SENT; //move onto next state
            }

            I2C1CONbits.ACKEN = 1; //send the ACK/NACK signal

            break;

        case ACK_SENT: //we have sent an ACK signal, prepare to receive another byte
            I2C1CONbits.RCEN = 1; //enable the receiver mode
            i2c_1_state = DATA_RECEIVED; //move onto next state
            break;

        case NOACK_SENT: //we have sent an NACK signal, prepare to stop transaction
            I2C1CONbits.PEN = 1; //send the stop signal

            //put node in receive buffer
            if (enqueue(&(i2c1.Rx_queue), (uint8*) & current_node, sizeof (current_node))) {
                //queue is full, error handling?
            }

            i2c_1_state = STOPPED; //move onto next state
            break;

        case STOPPED: //we have just sent the stop signal
            if (dequeue(&(i2c1.Tx_queue), (uint8*) & current_node, sizeof (current_node))) //load next node from the queue
            {
                i2c1.Tx_is_idle = TRUE; //flag that the bus is idle (nothing in the send queue)
            } else {
                I2C1CONbits.SEN = 1; //send the start signal
                i2c1.Tx_is_idle = FALSE; //flag that the bus is working now
                i2c_1_state = STARTED; //move onto next state
            }
            break;
    }

    if (I2C_1_callback != NULL) {
        I2C_1_callback();
    }

    asm volatile ("ei"); //reenable interrupts
}

void __ISR(_I2C_2_VECTOR, IPL7AUTO) I2C_2_Handler(void) {
    static I2C_Node current_node;
    static uint8 data_index;

    asm volatile ("di"); //disable interrupts

    IFS1bits.I2C2MIF = 0; //clear the interrupt flag


    switch (i2c_2_state) {
        case STARTED: //start signal has been sent, now to send the device addresss + write bit
            I2C2TRN = get_write_addr(current_node.device_address); //send device address + write bit

            //initialize indexes
            data_index = 0;
            //move on to next state
            i2c_2_state = DEV_ADDR_W_SENT;
            break;

        case DEV_ADDR_W_SENT: //device addresss + write bit has been sent, now to send the sub address
            I2C2TRN = current_node.sub_address; //send sub address byte
            i2c_2_state = SUB_ADDR_SENT; //move to next state
            break;

        case SUB_ADDR_SENT: //sub address has been sent, need logic to determine next step
            if (current_node.mode == READ) //if we want to read
            {
                I2C2CONbits.RSEN = 1; //send restart signal
                i2c_2_state = RESTARTED; //move onto next state
            } else //if we want to write
            {
                I2C2TRN = current_node.data_buffer[data_index]; //send first data byte
                i2c_2_state = DATA_SENT; //move on to next state
            }
            break;

        case DATA_SENT: //we have just sent a data byte
            ++data_index;

            if (data_index == current_node.data_size) //if we have sent all data bytes
            {
                I2C2CONbits.PEN = 1; //send the stop signal
                i2c_2_state = STOPPED; //move onto next state
            } else //if we have more bytes to send
            {
                I2C2TRN = current_node.data_buffer[data_index]; //start sending next bit
            }
            break;

        case RESTARTED: //restart signal has been sent, now to send device address + read bit
            I2C2TRN = get_read_addr(current_node.device_address); //send device address + read bit
            i2c_2_state = DEV_ADDR_R_SENT; //move onto next state
            break;

        case DEV_ADDR_R_SENT: //device address + read bit has been sent, get ready to receive byte(s)
            I2C2CONbits.RCEN = 1; //enable receiver mode
            i2c_2_state = DATA_RECEIVED; //move onto next state
            break;

        case DATA_RECEIVED: //we have just received a byte
            current_node.data_buffer[data_index] = I2C2RCV; //read the received data

            ++data_index;
            if (data_index == current_node.data_size) //if we have read all the data we want
            {
                I2C2CONbits.ACKDT = 1; //configure to send NACK signal
                i2c_2_state = NOACK_SENT; //move onto next state
            } else //we want to read another byte
            {
                I2C2CONbits.ACKDT = 0; //configure to send ACK signal
                i2c_2_state = ACK_SENT; //move onto next state
            }

            I2C2CONbits.ACKEN = 1; //send the ACK/NACK signal

            break;

        case ACK_SENT: //we have sent an ACK signal, prepare to receive another byte
            I2C2CONbits.RCEN = 1; //enable the receiver mode
            i2c_2_state = DATA_RECEIVED; //move onto next state
            break;

        case NOACK_SENT: //we have sent an NACK signal, prepare to stop transaction
            I2C2CONbits.PEN = 1; //send the stop signal

            //put node in receive buffer
            if (enqueue(&(i2c2.Rx_queue), (uint8*) & current_node, sizeof (current_node))) {
                //queue is full, error handling?
            }

            i2c_2_state = STOPPED; //move onto next state
            break;

        case STOPPED: //we have just sent the stop signal
            if (dequeue(&(i2c2.Tx_queue), (uint8*) & current_node, sizeof (current_node))) //load next node from the queue
            {
                i2c2.Tx_is_idle = TRUE; //flag that the bus is idle (nothing in the send queue)
            } else {
                I2C2CONbits.SEN = 1; //send the start signal
                i2c2.Tx_is_idle = FALSE; //flag that the bus is working now
                i2c_2_state = STARTED; //move onto next state
            }
            break;
    }

    if (I2C_2_callback != NULL) {
        I2C_2_callback();
    }

    asm volatile ("ei"); //reenable interrupts
}

/********************************************************
 *   File Name: functions.c
 *
 *   Description:
 *              Main source code file
 *
 *
 *********************************************************/
#include "system.h"
#include "functions.h"
#include "comm_UART_ISR.h"

/*************************************************************************
 Variables
 ************************************************************************/
COMM_UART_QUEUE COMM_UART_Queue;
boolean COMM_UART_is_idle;
boolean SYNC_LOCK;
uint8 received_bytes[6];
uint8 received_index;
boolean begin_sync;
boolean packet_recieved;

BG_COMM_UART_QUEUE BG_COMM_UART_Queue;

/********************************************************
 *   Function Name:
 *
 *   Description:
 *
 *
 *********************************************************/
void comm_uart_setup(void) {
    COMM_UART_BRG = 10; //divider of 10 for 56818.2 baud
    COMM_UART_PDSEL = 0;
    comm_uart_InitializeQueue(&COMM_UART_Queue);
    bg_comm_uart_setup();


    //Setup UART1 TX interrupts
    COMM_UART_TX_INT_set(1); //enable interrupt Tx
    COMM_UART_INT_PRIORITY_set(7); //set priority

    COMM_UART_UTXISEL = 2; //Set interrupt to fire when transmit buffer is empty
    COMM_UART_UTXEN = 1; //enable Tx

    //Setup UART1 RX interrupts
    COMM_UART_RX_INT_set(1); //enable interrupt Rx

    //set by default - U1STAbits.URXISEL = 0; //Set interrupt to fire when a character is recieved
    COMM_UART_URXEN = 1; //enable Rx

    COMM_UART_ON = 1;
    //    uart_InitializeQueue(&COMM_UART_Queue);//initialize the queue

    COMM_UART_is_idle = TRUE; //set that bus is currently idle
}

/********************************************************
 *   Function Name:
 *
 *   Description:
 *
 *
 *********************************************************/
inline void comm_uart_begin(void) {
    /* this sets the comm uart TX interrupt flag. Setting the flag will
     cause the ISR to be entered as soon as the global interrupt
     flag is enabled */
    COMM_UART_TX_INT_set(1); //enable interrupt Tx
    COMM_UART_TXIF = 1;
}

/********************************************************
 *   Function Name: comm_uart_InitializeQueue(COMM_UART_QUEUE* queue)
 *
 *   Description: Clears the queue and resets parameters
 *
 *
 *********************************************************/
void comm_uart_InitializeQueue(COMM_UART_QUEUE* queue) {
    memset(queue, 0, sizeof (COMM_UART_QUEUE));
}

/********************************************************
 *   Function Name: comm_uart_addToQueue(COMM_UART_QUEUE* queue, COMM_UART_NODE new_node)
 *
 *   Description: Adds a node to the queue - Pass a node by reference
 *
 *
 *********************************************************/
int comm_uart_addToQueue(COMM_UART_QUEUE* queue, COMM_UART_NODE new_node) {
    if (queue->QueueEnd == queue->QueueStart && queue->QueueLength > 0) {
        return 1; //Error, would overwrite start of list
    }
    queue->DataBank[queue->QueueEnd] = new_node;
    queue->QueueLength++;
    if (queue->QueueEnd == COMM_UARTQueueSize - 1) {
        queue->QueueEnd = 0;
    } else {
        queue->QueueEnd++;
    }
    return 0;
}

/********************************************************
 *   Function Name: comm_uart_popNode(COMM_UART_QUEUE* queue, COMM_UART_NODE* return_node)
 *
 *   Description: Pulls the next node off the queue
 *
 *
 *********************************************************/
int comm_uart_popNode(COMM_UART_QUEUE* queue, COMM_UART_NODE* return_node) {
    if (queue->QueueLength == 0) {
        return 1; //Can't read from queue if empty
    }
    *return_node = (queue->DataBank[queue->QueueStart]); //Returns the Node
    if (queue->QueueStart == COMM_UARTQueueSize - 1) {
        queue->QueueStart = 0;
    } else {
        queue->QueueStart++;
    }
    queue->QueueLength--;
    return 0;
}


/********************************************************
 *   Function Name: comm_uart_CreateNode(uint Byte1, uint Byte2, uint Byte3)
 *
 *   Description: Creates a node using three bytes
 *
 *
 *********************************************************/
void comm_uart_CreateNode(uint Byte1, uint Byte2, uint Byte3) {
    COMM_UART_NODE temp;

    temp.uart_data[0] = '\n';
    temp.uart_data[1] = Byte1;
    temp.uart_data[2] = Byte2;
    temp.uart_data[3] = Byte3;


    comm_uart_addToQueue(&COMM_UART_Queue, temp);
}

/********************************************************
 *   Function Name: comm_uart_Handler
 *
 *   Description:
 *             ISR for the COMM_UART module
 *
 *********************************************************/
void __ISR(_COMM_UART_VECTOR, IPL7AUTO) comm_uart_Handler(void) {
    int i, k;
    COMM_UART_NODE current_node;
    uint8 received_byte;

    INTDisableInterrupts();

    //URXDA is 1 if recieve buffer has data
    //TRMT is 1 if transmit buffer is empty
    if (COMM_UART_RXIF == 1) {
        
        received_byte = COMM_UART_RXREG;
        bg_comm_uart_CreateNode(received_byte);

        //COMM_UART_RXIF is the recieve register that data will come into
        COMM_UART_RXIF = 0; //clear the interrupt flag
    }
    if (COMM_UART_TXIF == 1) {
        if (comm_uart_popNode(&COMM_UART_Queue, &current_node)) {
            COMM_UART_is_idle = TRUE;
            //disable the interrupt
            COMM_UART_TX_INT_clr(1); //disable interrupt Tx (this is because it constantly fires)
        } else {
            for (i = 0; i < 4; i++) {
                COMM_UART_TXREG = current_node.uart_data[i];
            }
            COMM_UART_is_idle = FALSE;
        }

        COMM_UART_TXIF = 0; //clear the interrupt flag
    }

    INTEnableInterrupts();

}
/********************************************************
 *   Function Name: bg_proc_comm_uart()
 *
 *   Description: background processing for the comm_uart
 *
 *
 *********************************************************/
void bg_process_comm_uart(void) {
    extern boolean MOTOR_UART_is_idle;
    uint8 received_byte;
    BG_COMM_UART_NODE temp_node;



    if (bg_comm_uart_popNode(&BG_COMM_UART_Queue, &temp_node)) //Returns a 1 if empty
    {
        //do nothing - the queue is empty
    }
    else {

        received_byte = temp_node.uart_data;

        if (SYNC_LOCK) //if in sync
        {

            //PORTAbits.RA9 = 1;
            if (received_index == 0)//only check the first byte for the control byte
            {
                packet_recieved = FALSE; //start of a new packet
                if (received_byte != CONTROL_BYTE) {
                    SYNC_LOCK = FALSE;
                    begin_sync = TRUE;
                    //PORTAbits.RA9 = 0;

                    //CALL SYNC FUNCTION
                }
                received_index++; //assuming we are synced, so dont add a new character to the packet
            } else if (received_index == 1) //NOTE only indexes 1 and 2 are used
            {
                received_bytes[received_index] = received_byte;
                received_index++;
            } else if (received_index == 2) {
                received_bytes[received_index] = received_byte;
                received_index = 0; //Reset the index, we now have the full package
                packet_recieved = TRUE;

            }
        } else {
            if (begin_sync == TRUE) {
                received_index = 0;
                if (received_byte == CONTROL_BYTE) {
                    begin_sync = FALSE;
                }
            } 
            
            if (begin_sync == FALSE){

                received_bytes[received_index] = received_byte;
                ++received_index;

                //X00X00
                //012345
                if (received_index == 6) {
                    if (received_bytes[0] == CONTROL_BYTE &&
                            received_bytes[3] == CONTROL_BYTE
                            ) {
                        SYNC_LOCK = TRUE;
                        received_index = 0;

                    } else {
                        begin_sync = TRUE;

                    }
                }
            }
        }

        //MOVE THIS TO THE MAIN PROCESSING FUNCTION
        if (packet_recieved) {

            switch (received_bytes[1]) {
                case THRUSTER_BOW_SB:
                    if (received_bytes[2] & 0x80) //Pull off the direction bit
                    {
                        Motor1_Forward(129, (received_bytes[2] & 0x7F));
                    } else {
                        Motor1_Backward(129, (received_bytes[2] & 0x7F));
                    }
                    break;
                case THRUSTER_BOW_PORT:
                    if (received_bytes[2] & 0x80) //Pull off the direction bit
                    {
                        Motor1_Forward(128, (received_bytes[2] & 0x7F));
                    } else {
                        Motor1_Backward(128, (received_bytes[2] & 0x7F));
                    }
                    break;
                case THRUSTER_STERN_SB:
                    if (received_bytes[2] & 0x80) //Pull off the direction bit
                    {
                        Motor2_Forward(129, (received_bytes[2] & 0x7F));
                    } else {
                        Motor2_Backward(129, (received_bytes[2] & 0x7F));
                    }
                    break;
                case THRUSTER_STERN_PORT:
                    if (received_bytes[2] & 0x80) //Pull off the direction bit
                    {
                        Motor2_Forward(128, (received_bytes[2] & 0x7F));
                    } else {
                        Motor2_Backward(128, (received_bytes[2] & 0x7F));
                    }
                    break;
                case THRUSTER_DEPTH_SB:
                    if (received_bytes[2] & 0x80) //Pull off the direction bit
                    {
                        Motor2_Forward(130, (received_bytes[2] & 0x7F));
                    } else {
                        Motor2_Backward(130, (received_bytes[2] & 0x7F));
                    }
                    break;
                case THRUSTER_DEPTH_PORT:
                    if (received_bytes[2] & 0x80) //Pull off the direction bit
                    {
                        Motor1_Forward(130, (received_bytes[2] & 0x7F));
                    } else {
                        Motor1_Backward(130, (received_bytes[2] & 0x7F));
                    }
                    break;

            }

            if (MOTOR_UART_is_idle) {
                motor_uart_begin();

            }
        }
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
void bg_comm_uart_setup(void) {

    bg_comm_uart_InitializeQueue(&BG_COMM_UART_Queue);
    //initialize any background process variables if necessary
    //initialize variables
    memset(received_bytes, 0, sizeof (received_bytes));
    received_index = 0;
    packet_recieved = FALSE;
    SYNC_LOCK = FALSE;
    begin_sync = TRUE;
}


/********************************************************
 *   Function Name: bg_comm_uart_InitializeQueue(BG_COMM_UART_QUEUE* queue)
 *
 *   Description: Clears the queue and resets parameters
 *
 *
 *********************************************************/
void bg_comm_uart_InitializeQueue(BG_COMM_UART_QUEUE* queue) {
    memset(queue, 0, sizeof (BG_COMM_UART_QUEUE));
}

/********************************************************
 *   Function Name: bg_comm_uart_addToQueue(BG_COMM_UART_QUEUE* queue, BG_COMM_UART_NODE new_node)
 *
 *   Description: Adds a node to the queue - Pass a node by reference
 *
 *
 *********************************************************/
int bg_comm_uart_addToQueue(BG_COMM_UART_QUEUE* queue, BG_COMM_UART_NODE new_node) {
    if (queue->QueueEnd == queue->QueueStart && queue->QueueLength > 0) {
        return 1; //Error, would overwrite start of list
    }
    queue->DataBank[queue->QueueEnd] = new_node;
    queue->QueueLength++;
    if (queue->QueueEnd == BG_COMM_UARTQueueSize - 1) {
        queue->QueueEnd = 0;
    } else {
        queue->QueueEnd++;
    }
    return 0;
}

/********************************************************
 *   Function Name: bg_comm_uart_popNode(BG_COMM_UART_QUEUE* queue, BG_COMM_UART_NODE* return_node)
 *
 *   Description: Pulls the next node off the queue
 *
 *
 *********************************************************/
int bg_comm_uart_popNode(BG_COMM_UART_QUEUE* queue, BG_COMM_UART_NODE* return_node) {
    if (queue->QueueLength == 0) {
        return 1; //Can't read from queue if empty
    }
    *return_node = (queue->DataBank[queue->QueueStart]); //Returns the Node
    if (queue->QueueStart == BG_COMM_UARTQueueSize - 1) {
        queue->QueueStart = 0;
    } else {
        queue->QueueStart++;
    }
    queue->QueueLength--;
    return 0;
}


/********************************************************
 *   Function Name: comm_uart_SetNode( uint Byte1, uint Byte2, uint Byte3 )
 *
 *   Description: Creates a node using three bytes
 *
 *
 *********************************************************/
void  bg_comm_uart_CreateNode( uint8 received_byte ) {
    BG_COMM_UART_NODE temp;

    temp.uart_data = received_byte;

    bg_comm_uart_addToQueue(&BG_COMM_UART_Queue, temp);
}
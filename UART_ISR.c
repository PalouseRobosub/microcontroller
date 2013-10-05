/********************************************************
 *   File Name: functions.c
 *
 *   Description:
 *              Main source code file
 *
 *
 *********************************************************/
#include "functions.h"
#include "UART_ISR.h"

/*************************************************************************
 Variables
 ************************************************************************/
UART_QUEUE UART_1_Queue;
boolean UART1_is_idle;

/********************************************************
 *   Function Name:
 *
 *   Description:
 *
 *
 *********************************************************/
void uart_setup(void) {
    U1BRG = 64; //if main clock is 40 MHz, use  259. 10MHz, use 64
    U1MODEbits.PDSEL = 0;
    uart_InitializeQueue(&UART_1_Queue);


    //Setup UART1 TX interrupts
    IEC0SET = (1 << 28); //enable interrupt Tx
    IPC6SET = (2 << 2); //set priority
    U1STAbits.UTXISEL = 2; //Set interrupt to fire when transmit buffer is empty
    U1STAbits.UTXEN = 1; //enable Tx
    
    //Setup UART1 RX interrupts
    IEC0SET = (1 << 27); //enable interrupt Rx
    //set by default - U1STAbits.URXISEL = 0; //Set interrupt to fire when a character is recieved
    U1STAbits.URXEN = 1; //enable Rx

    U1MODEbits.ON = 1;
    //    uart_InitializeQueue(&UART_1_Queue);//initialize the queue

    UART1_is_idle = TRUE; //set that bus is currently idle
}

/********************************************************
 *   Function Name:
 *
 *   Description:
 *
 *
 *********************************************************/
inline void uart_begin(void) {
    /* this sets the UART1TX interrupt flag. Setting the flag will
     cause the ISR to be entered as soon as the global interrupt
     flag is enabled */
    IFS0bits.U1TXIF = 1;
}

/********************************************************
 *   Function Name: uart_InitializeQueue(UART_QUEUE* queue)
 *
 *   Description: Clears the queue and resets parameters
 *
 *
 *********************************************************/
void uart_InitializeQueue(UART_QUEUE* queue) {
    memset(queue, 0, sizeof (UART_QUEUE));
}

/********************************************************
 *   Function Name: uart_addToQueue(UART_QUEUE* queue, UART_NODE new_node)
 *
 *   Description: Adds a node to the queue - Pass a node by reference
 *
 *
 *********************************************************/
int uart_addToQueue(UART_QUEUE* queue, UART_NODE new_node) {
    if (queue->QueueEnd == queue->QueueStart && queue->QueueLength > 0) {
        return 1; //Error, would overwrite start of list
    }
    queue->DataBank[queue->QueueEnd] = new_node;
    queue->QueueLength++;
    if (queue->QueueEnd == UARTQueueSize - 1) {
        queue->QueueEnd = 0;
    } else {
        queue->QueueEnd++;
    }
    return 0;
}

/********************************************************
 *   Function Name: uart_popNode(UART_QUEUE* queue, UART_NODE* return_node)
 *
 *   Description: Pulls the next node off the queue
 *
 *
 *********************************************************/
int uart_popNode(UART_QUEUE* queue, UART_NODE* return_node) {
    if (queue->QueueLength == 0) {
        return 1; //Can't read from queue if empty
    }
    *return_node = (queue->DataBank[queue->QueueStart]); //Returns the Node
    if (queue->QueueStart == UARTQueueSize - 1) {
        queue->QueueStart = 0;
    } else {
        queue->QueueStart++;
    }
    queue->QueueLength--;
    return 0;
}

/********************************************************
 *   Function Name: _UART_1_Handler
 *
 *   Description:
 *             ISR for the UART1 module
 *
 *********************************************************/
void __ISR(_UART1_VECTOR, ipl2) IntUart1Handler(void) {
    int i, k;
    static UART_NODE current_node;
    static uint8 data_index;

    //URXDA is 1 if recieve buffer has data
    //TRMT is 1 if transmit buffer is empty

    if (IFS0bits.U1RXIF == 1) {

        //U1RXREG is the recieve register that data will come into
        IFS0bits.U1RXIF = 0; //clear the interrupt flag
    }
    if (IFS0bits.U1TXIF == 1) {

        ++data_index;

         if (data_index == 4) //if we have sent all data bytes
         {
             data_index = 0;
         }
         else //if we have more bytes to send
         {
             U1TXREG = current_node.uart_data[data_index];
         }
        /*if (uart_popNode(&UART_1_Queue, &current_node)) {
            UART1_is_idle = TRUE;
        } else {
            for (i = 0; i < 4; i++) {
                //            U1STAbits.UTXBRK = 1;
                U1TXREG = current_node.uart_data[i];
                for (k = 0; k < 500; k++); //delay to keep the FIFO from overflowing
                //            U1STAbits.UTXBRK = 0;
            }
            UART1_is_idle = FALSE;
        }*/

        IFS0bits.U1TXIF = 0; //clear the interrupt flag
    }

}

/********************************************************
 *   Function Name: uart_SetNode( uint Byte1, uint Byte2, uint Byte3 )
 *
 *   Description: Creates a node using three bytes
 *
 *
 *********************************************************/
void uart_CreateNode(uint Byte1, uint Byte2, uint Byte3) {
    UART_NODE temp;

    temp.uart_data[0] = Byte1;
    temp.uart_data[1] = Byte2;
    temp.uart_data[2] = Byte3;
    temp.uart_data[3] = '\n';

    uart_addToQueue(&UART_1_Queue, temp);
}
/********************************************************
 *   File Name: functions.c
 *
 *   Description:
 *              Main source code file
 *
 *
 *********************************************************/
#include "functions.h"
#include "comm_UART_ISR.h"

/*************************************************************************
 Variables
 ************************************************************************/
UART1_QUEUE UART_1_Queue;
boolean UART1_is_idle;
uint8 led_val;

/********************************************************
 *   Function Name:
 *
 *   Description:
 *
 *
 *********************************************************/
void uart1_setup(void) {
    //U1BRG = 64; //divider of 64 for 9600 baud
    U1BRG = 10; //divider of 10 for 56818.2 baud
    U1MODEbits.PDSEL = 0;
    uart1_InitializeQueue(&UART_1_Queue);
    led_val = 0;


    //Setup UART1 TX interrupts
    IEC0SET = (1 << 28); //enable interrupt Tx
    IPC6SET = (7 << 2); //set priority
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
inline void uart1_begin(void) {
    /* this sets the UART1TX interrupt flag. Setting the flag will
     cause the ISR to be entered as soon as the global interrupt
     flag is enabled */
    IFS0bits.U1TXIF = 1;
}

/********************************************************
 *   Function Name: uart1_InitializeQueue(UART1_QUEUE* queue)
 *
 *   Description: Clears the queue and resets parameters
 *
 *
 *********************************************************/
void uart1_InitializeQueue(UART1_QUEUE* queue) {
    memset(queue, 0, sizeof (UART1_QUEUE));
}

/********************************************************
 *   Function Name: uart1_addToQueue(UART1_QUEUE* queue, UART1_NODE new_node)
 *
 *   Description: Adds a node to the queue - Pass a node by reference
 *
 *
 *********************************************************/
int uart1_addToQueue(UART1_QUEUE* queue, UART1_NODE new_node) {
    if (queue->QueueEnd == queue->QueueStart && queue->QueueLength > 0) {
        return 1; //Error, would overwrite start of list
    }
    queue->DataBank[queue->QueueEnd] = new_node;
    queue->QueueLength++;
    if (queue->QueueEnd == UART1QueueSize - 1) {
        queue->QueueEnd = 0;
    } else {
        queue->QueueEnd++;
    }
    return 0;
}

/********************************************************
 *   Function Name: uart1_popNode(UART1_QUEUE* queue, UART1_NODE* return_node)
 *
 *   Description: Pulls the next node off the queue
 *
 *
 *********************************************************/
int uart1_popNode(UART1_QUEUE* queue, UART1_NODE* return_node) {
    if (queue->QueueLength == 0) {
        return 1; //Can't read from queue if empty
    }
    *return_node = (queue->DataBank[queue->QueueStart]); //Returns the Node
    if (queue->QueueStart == UART1QueueSize - 1) {
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
void __ISR(_UART1_VECTOR, IPL7AUTO) IntUart1Handler(void) {
    int i, k;
    UART1_NODE current_node;
    INTDisableInterrupts();
    uint8 received_byte;


    //write_leds(led_val);
    //led_val = ~led_val;
    //URXDA is 1 if recieve buffer has data
    //TRMT is 1 if transmit buffer is empty

    if (IFS0bits.U1RXIF == 1) {
        received_byte = U1RXREG;
        write_leds(received_byte - '0'); //write to LEDs to test UART Rx
        if (received_byte == 'P')
        {
            uart1_CreateNode('P', 0, 0);
            uart1_CreateNode('Q', UART_1_Queue.QueueLength, 0);
           // write_leds(0xFF);
        }

        //U1RXREG is the recieve register that data will come into
        IFS0bits.U1RXIF = 0; //clear the interrupt flag
    }
    if (IFS0bits.U1TXIF == 1) {
        if (uart1_popNode(&UART_1_Queue, &current_node)) {
            UART1_is_idle = TRUE;
        } else {
            for (i = 0; i < 4; i++) {
                //            U1STAbits.UTXBRK = 1;
                U1TXREG = current_node.uart_data[i];
                //            U1STAbits.UTXBRK = 0;
            }
            UART1_is_idle = FALSE;
        }

        IFS0bits.U1TXIF = 0; //clear the interrupt flag
    }

    INTEnableInterrupts();

}

/********************************************************
 *   Function Name: uart1_SetNode( uint Byte1, uint Byte2, uint Byte3 )
 *
 *   Description: Creates a node using three bytes
 *
 *
 *********************************************************/
void uart1_CreateNode(uint Byte1, uint Byte2, uint Byte3) {
    UART1_NODE temp;

    temp.uart_data[0] = '\n';
    temp.uart_data[1] = Byte1;
    temp.uart_data[2] = Byte2;
    temp.uart_data[3] = Byte3;
    

    uart1_addToQueue(&UART_1_Queue, temp);
}
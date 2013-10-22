/********************************************************
 *   File Name: functions.c
 *
 *   Description:
 *              Main source code file
 *
 *
 *********************************************************/
#include "functions.h"
#include "motor_UART_ISR.h"

/*************************************************************************
 Variables
 ************************************************************************/
UART2_QUEUE UART_2_Queue;
boolean UART2_is_idle;
uint8 led_val;

/********************************************************
 *   Function Name:
 *
 *   Description:
 *
 *
 *********************************************************/
void uart2_setup(void) {
    //U1BRG = 64;  //divider of 10 for 56818.2 baud
    U2BRG = 64; //divider of 64 for 9600 baud (default for motors)
    U2MODEbits.PDSEL = 0;
    uart2_InitializeQueue(&UART_2_Queue);
    led_val = 0;


    //Setup UART2 TX interrupts
    IEC1SET = (1 << 10); //enable interrupt Tx
    IPC8SET = (7 << 2); //set priority
    U2STAbits.UTXISEL = 2; //Set interrupt to fire when transmit buffer is empty
    U2STAbits.UTXEN = 1; //enable Tx

    //Setup UART1 RX interrupts
    IEC1SET = (1 << 9); //enable interrupt Rx
    //set by default - U1STAbits.URXISEL = 0; //Set interrupt to fire when a character is recieved
    U2STAbits.URXEN = 1; //enable Rx

    U2MODEbits.ON = 1;
    //    uart2_InitializeQueue(&UART_2_Queue);//initialize the queue

    UART2_is_idle = TRUE; //set that bus is currently idle
}

/********************************************************
 *   Function Name:
 *
 *   Description:
 *
 *
 *********************************************************/
inline void uart2_begin(void) {
    /* this sets the UART1TX interrupt flag. Setting the flag will
     cause the ISR to be entered as soon as the global interrupt
     flag is enabled */
    IFS1bits.U2TXIF = 1;
}

/********************************************************
 *   Function Name: uart2_InitializeQueue(UART2_QUEUE* queue)
 *
 *   Description: Clears the queue and resets parameters
 *
 *
 *********************************************************/
void uart2_InitializeQueue(UART2_QUEUE* queue) {
    memset(queue, 0, sizeof (UART2_QUEUE));
}

/********************************************************
 *   Function Name: uart2_addToQueue(UART2_QUEUE* queue, UART2_NODE new_node)
 *
 *   Description: Adds a node to the queue - Pass a node by reference
 *
 *
 *********************************************************/
int uart2_addToQueue(UART2_QUEUE* queue, UART2_NODE new_node) {
    if (queue->QueueEnd == queue->QueueStart && queue->QueueLength > 0) {
        return 1; //Error, would overwrite start of list
    }
    queue->DataBank[queue->QueueEnd] = new_node;
    queue->QueueLength++;
    if (queue->QueueEnd == UART2QueueSize - 1) {
        queue->QueueEnd = 0;
    } else {
        queue->QueueEnd++;
    }
    return 0;
}

/********************************************************
 *   Function Name: uart2_popNode(UART2_QUEUE* queue, UART2_NODE* return_node)
 *
 *   Description: Pulls the next node off the queue
 *
 *
 *********************************************************/
int uart2_popNode(UART2_QUEUE* queue, UART2_NODE* return_node) {
    if (queue->QueueLength == 0) {
        return 1; //Can't read from queue if empty
    }
    *return_node = (queue->DataBank[queue->QueueStart]); //Returns the Node
    if (queue->QueueStart == UART2QueueSize - 1) {
        queue->QueueStart = 0;
    } else {
        queue->QueueStart++;
    }
    queue->QueueLength--;
    return 0;
}

/********************************************************
 *   Function Name: _UART_2_Handler
 *
 *   Description:
 *             ISR for the UART1 module
 *
 *********************************************************/
void __ISR(_UART2_VECTOR, IPL7AUTO) IntUart2Handler(void) {
    int i, k;
    UART2_NODE current_node;
    INTDisableInterrupts();
    uint8 received_byte;


    //write_leds(led_val);
    //led_val = ~led_val;
    //URXDA is 1 if recieve buffer has data
    //TRMT is 1 if transmit buffer is empty

    if (IFS1bits.U2RXIF == 1) {
        received_byte = U2RXREG;
        write_leds(received_byte - '0'); //write to LEDs to test UART Rx
        if (received_byte == 'P')
        {
            uart2_CreateNode('P', 0, 0);
            uart2_CreateNode('Q', UART_2_Queue.QueueLength, 0);
           // write_leds(0xFF);
        }

        //U1RXREG is the recieve register that data will come into
        IFS1bits.U2RXIF = 0; //clear the interrupt flag
    }
    if (IFS1bits.U2TXIF == 1) {
        if (uart2_popNode(&UART_2_Queue, &current_node)) {
            UART2_is_idle = TRUE;
        } else {
            for (i = 0; i < 4; i++) {
                //            U1STAbits.UTXBRK = 1;
                U2TXREG = current_node.uart_data[i];
                //            U1STAbits.UTXBRK = 0;
            }
            UART2_is_idle = FALSE;
        }

        IFS1bits.U2TXIF = 0; //clear the interrupt flag
    }

    INTEnableInterrupts();

}

/********************************************************
 *   Function Name: uart2_SetNode( uint Byte1, uint Byte2, uint Byte3 )
 *
 *   Description: Creates a node using three bytes
 *
 *
 *********************************************************/
void uart2_CreateNode(uint Byte1, uint Byte2, uint Byte3) {
    UART2_NODE temp;

    temp.uart_data[0] = '\n';
    temp.uart_data[1] = Byte1;
    temp.uart_data[2] = Byte2;
    temp.uart_data[3] = Byte3;
    

    uart2_addToQueue(&UART_2_Queue, temp);
}
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
void uart_setup(UART_MODULE uart_id)
{
    U1BRG = 259; //if main clock is 40 MHz, use  259. 10MHz, use 64
    U1MODEbits.ON = 1;
    U1MODEbits.PDSEL = 0;


    //Setup UART1 TX interrupts
    IEC0SET = (1 << 28); //enable interrupt
    IPC6SET = (2 << 2); //set priority
    
//    uart_InitializeQueue(&UART_1_Queue);//initialize the queue

    UART1_is_idle = TRUE;//set that bus is currently idle
}

/********************************************************
 *   Function Name:
 *
 *   Description:
 *
 *
 *********************************************************/
inline void uart_begin(void)
{
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
void uart_InitializeQueue(UART_QUEUE* queue)
{
    memset(queue, 0, sizeof(UART_QUEUE));
}

/********************************************************
 *   Function Name: uart_addToQueue(UART_QUEUE* queue, UART_NODE new_node)
 *
 *   Description: Adds a node to the queue - Pass a node by reference
 *
 *
 *********************************************************/
int uart_addToQueue(UART_QUEUE* queue, UART_NODE new_node)
{
    if (queue->QueueEnd == queue->QueueStart && queue->QueueLength > 0)
    {
        return 1; //Error, would overwrite start of list
    }
    queue->DataBank[queue->QueueEnd] = new_node;
    queue->QueueLength++;
    if (queue->QueueEnd == UARTQueueSize-1)
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
 *   Function Name: uart_popNode(UART_QUEUE* queue, UART_NODE* return_node)
 *
 *   Description: Pulls the next node off the queue
 *
 *
 *********************************************************/
int uart_popNode(UART_QUEUE* queue, UART_NODE* return_node)
{
    if (queue->QueueLength == 0)
    {
        return 1; //Can't read from queue if empty
    }
    *return_node = (queue->DataBank[queue->QueueStart]); //Returns the Node
    if (queue->QueueStart == UARTQueueSize-1)
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

 /********************************************************
 *   Function Name:
 *
 *   Description:
 *
 *
 *********************************************************/
void send_data(UART_MODULE uart_id, UINT8* bsndData, UINT16 size)
{
    int i;

    for(i = 0; i < size; i++)
    {
        while(!UARTTransmitterIsReady(uart_id));

        UARTSendDataByte(uart_id, bsndData[i]);

        while(!UARTTransmissionHasCompleted(uart_id));
    }
}

 /********************************************************
 *   Function Name:
 *
 *   Description:
 *
 *
 *********************************************************/
void read_data(UART_MODULE uart_id)
{
    int i = 0;

    while(UARTReceivedDataIsAvailable(uart_id))
    {
        brecvData[i] = UARTGetDataByte(uart_id);
        i++;
    }

}

 /********************************************************
 *   Function Name: _UART_1_Handler
 *
 *   Description:
 *             ISR for the UART1 module
 *
 *********************************************************/
void __ISR(_UART1_VECTOR, ipl2) IntUart1Handler(void)
{
    int i;

    UART_NODE current_node;
    current_node.uart_data[0] = 'a';
    current_node.uart_data[1] = 'b';
    current_node.uart_data[2] = 'c';
    current_node.uart_data[3] = 'd';

//    if(uart_popNode(&UART_1_Queue, &current_node))
//    {
//        UART1_is_idle = TRUE;
//    }
//    else
//    {
        for(i = 0; i < 4; i++)
        {
            U1STAbits.UTXEN = 1;
            U1STAbits.UTXBRK = 1;
            U1TXREG = 0xFF;
            //U1TXREG = current_node.uart_data[i];
            U1STAbits.UTXBRK = 0;
        }
        UART1_is_idle = FALSE;
//    }

        IFS0bits.U1TXIF = 0; //clear the interrupt flag
}
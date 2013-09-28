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

    //Setup UART1 TX interrupts
    IEC0SET = (1 << 28); //enable interrupt
    IPC6SET = (2 << 2); //set priority
    
    uart_InitializeQueue(&UART_1_Queue);//initialize the queue

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

}

/********************************************************
 *   Function Name:
 *
 *   Description:
 *
 *
 *********************************************************/
void uart_InitializeQueue(UART_QUEUE* queue)
{

}

/********************************************************
 *   Function Name:
 *
 *   Description:
 *
 *
 *********************************************************/
int uart_addToQueue(UART_QUEUE* queue, UART_NODE new_node)
{

}

/********************************************************
 *   Function Name:
 *
 *   Description:
 *
 *
 *********************************************************/
int uart_popNode(UART_QUEUE* queue, UART_NODE* return_node)
{

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
 *   Function Name:
 *
 *   Description:
 *
 *
 *********************************************************/
void __ISR(_UART1_VECTOR, ipl2) IntUart1Handler(void)
{
	// Is this an RX interrupt?
	if(INTGetFlag(INT_SOURCE_UART_RX(UART_ID)))
	{
            // Clear the RX interrupt Flag
	    INTClearFlag(INT_SOURCE_UART_RX(UART_ID));

            //get data
            read_data(UART_ID);

            // Echo what we just received.
            send_data(UART_ID, brecvData, sizeof(brecvData));
	}

	// TX interrupt
	if ( INTGetFlag(INT_SOURCE_UART_TX(UART_ID)) )
	{
            // Clear the TX interrupt Flag
            INTClearFlag(INT_SOURCE_UART_TX(UART_ID));

	}
}
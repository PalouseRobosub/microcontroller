/********************************************************
 *   File Name: functions.c
 *
 *   Description:
 *              Main source code file
 *
 *
 *********************************************************/



/*************************************************************************
 System Includes
 ************************************************************************/
#include "system.h"
#include "motor_UART_ISR.h"

#if defined (COMPILE_OLD_SUB)
/*************************************************************************
 Variables
 ************************************************************************/
MOTOR_UART_QUEUE MOTOR_UART_Queue;
boolean MOTOR_UART_is_idle;

/********************************************************
 *   Function Name:
 *
 *   Description:
 *
 *
 *********************************************************/
void motor_uart_setup(void) {
    
    MOTOR_UART_BRG = 64; //divider of 64 for 9600 baud (default for motors)
    MOTOR_UART_PDSEL = 0;
    motor_uart_InitializeQueue(&MOTOR_UART_Queue);


    //Setup TX interrupts
    MOTOR_UART_TX_INT_set(1); //enable interrupt Tx
    MOTOR_UART_INT_PRIORITY_set(7); //set priority
    MOTOR_UART_UTXISEL = 2; //Set interrupt to fire when transmit buffer is empty
    MOTOR_UART_UTXEN = 1; //enable Tx


    MOTOR_UART_ON = 1;

    MOTOR_UART_is_idle = TRUE; //set that bus is currently idle
}

/********************************************************
 *   Function Name:
 *
 *   Description:
 *
 *
 *********************************************************/
inline void motor_uart_begin(void) {
    /* this sets the UART TX interrupt flag. Setting the flag will
     cause the ISR to be entered as soon as the global interrupt
     flag is enabled */
    MOTOR_UART_TX_INT_set(1); //enable interrupt Tx
    MOTOR_UART_TXIF = 1;
}

/********************************************************
 *   Function Name: motor_uart_InitializeQueue(MOTOR_UART_QUEUE* queue)
 *
 *   Description: Clears the queue and resets parameters
 *
 *
 *********************************************************/
void motor_uart_InitializeQueue(MOTOR_UART_QUEUE* queue) {
    memset(queue, 0, sizeof (MOTOR_UART_QUEUE));
}

/********************************************************
 *   Function Name: motor_uart_addToQueue(MOTOR_UART_QUEUE* queue, MOTOR_UART_NODE new_node)
 *
 *   Description: Adds a node to the queue - Pass a node by reference
 *
 *
 *********************************************************/
int motor_uart_addToQueue(MOTOR_UART_QUEUE* queue, MOTOR_UART_NODE new_node) {
    
    if (queue->QueueEnd == queue->QueueStart && queue->QueueLength > 0) {
        return 1; //Error, would overwrite start of list
    }
    queue->DataBank[queue->QueueEnd] = new_node;
    queue->QueueLength++;
    if (queue->QueueEnd == MOTOR_UARTQueueSize - 1) {
        queue->QueueEnd = 0;
    } else {
        queue->QueueEnd++;
    }
    PORTGbits.RG12 = 1;
    return 0;
}

/********************************************************
 *   Function Name: motor_uart_popNode(MOTOR_UART_QUEUE* queue, MOTOR_UART_NODE* return_node)
 *
 *   Description: Pulls the next node off the queue
 *
 *
 *********************************************************/
int motor_uart_popNode(MOTOR_UART_QUEUE* queue, MOTOR_UART_NODE* return_node) {
    
    if (queue->QueueLength == 0) {
        return 1; //Can't read from queue if empty
    }
    *return_node = (queue->DataBank[queue->QueueStart]); //Returns the Node
    if (queue->QueueStart == MOTOR_UARTQueueSize - 1) {
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
void __ISR(_MOTOR_UART_VECTOR, IPL7AUTO) motor_uart_Handler(void) {
    int i;
    MOTOR_UART_NODE current_node;

    INTDisableInterrupts();
   
    
    if (MOTOR_UART_TXIF == 1) {
        if (motor_uart_popNode(&MOTOR_UART_Queue, &current_node)) {
            MOTOR_UART_is_idle = TRUE;
            MOTOR_UART_TX_INT_clr(1); //disable the interrupt Tx
        } else {
            for (i = 0; i < 4; i++) {
                //Transmit one byte at a time until the full packet is sent
                MOTOR_UART_TXREG = current_node.uart_data[i];
            }
            MOTOR_UART_is_idle = FALSE;
        }

        MOTOR_UART_TXIF = 0; //clear the interrupt flag
    }

    INTEnableInterrupts();

}

/********************************************************
 *   Function Name: motor_uart_SetNode( uint Byte1, uint Byte2, uint Byte3 )
 *
 *   Description: Creates a node using three bytes
 *
 *
 *********************************************************/
void motor_uart_CreateNode(uint Byte1, uint Byte2, uint Byte3, uint Byte4) {
    MOTOR_UART_NODE temp;

    temp.uart_data[0] = Byte1; //address - set on the H-bridge switches
    temp.uart_data[1] = Byte2; //direction
    temp.uart_data[2] = Byte3; //speed - integer from 0-127
    temp.uart_data[3] = Byte4; //7-bit checksum

    

    motor_uart_addToQueue(&MOTOR_UART_Queue, temp);
}

 /********************************************************
 *   Function Name: Motor1_Forward( uint address, uint speed )
 *
 *   Description: Create a node to move motor 1 forward
 *
 *
 *********************************************************/
void Motor1_Forward( uint address, uint speed )
{
    MOTOR_UART_NODE temp;
    int checksum = 0;

    checksum = ((address + 0 + speed) & 127);

    temp.uart_data[0] = address; //address - set on the H-bridge switches
    temp.uart_data[1] = 0; //direction
    temp.uart_data[2] = speed; //speed - integer from 0-127
    temp.uart_data[3] = checksum; //7-bit checksum

    motor_uart_addToQueue(&MOTOR_UART_Queue, temp);
}

 /********************************************************
 *   Function Name: Motor1_Backward( uint address, uint speed )
 *
 *   Description: Create a node to move motor 1 backward
 *
 *
 *********************************************************/
void Motor1_Backward( uint address, uint speed )
{
    MOTOR_UART_NODE temp;
    int checksum = 0;

    checksum = ((address + 1 + speed) & 127);

    temp.uart_data[0] = address; //address - set on the H-bridge switches
    temp.uart_data[1] = 1; //direction
    temp.uart_data[2] = speed; //speed - integer from 0-127
    temp.uart_data[3] = checksum; //7-bit checksum

    motor_uart_addToQueue(&MOTOR_UART_Queue, temp);
}

 /********************************************************
 *   Function Name: Motor1_Forward( uint address, uint speed )
 *
 *   Description: Create a node to move motor 2 forward
 *
 *
 *********************************************************/
void Motor2_Forward( uint address, uint speed )
{
    MOTOR_UART_NODE temp;
    int checksum = 0;

    checksum = ((address + 4 + speed) & 127);

    temp.uart_data[0] = address; //address - set on the H-bridge switches
    temp.uart_data[1] = 4; //direction
    temp.uart_data[2] = speed; //speed - integer from 0-127
    temp.uart_data[3] = checksum; //7-bit checksum

    motor_uart_addToQueue(&MOTOR_UART_Queue, temp);
}

 /********************************************************
 *   Function Name: Motor1_Backward( uint address, uint speed )
 *
 *   Description: Create a node to move motor 2 backward
 *
 *
 *********************************************************/
void Motor2_Backward( uint address, uint speed )
{
    MOTOR_UART_NODE temp;
    int checksum = 0;

    checksum = ((address + 5 + speed) & 127);

    temp.uart_data[0] = address; //address - set on the H-bridge switches
    temp.uart_data[1] = 5; //direction
    temp.uart_data[2] = speed; //speed - integer from 0-127
    temp.uart_data[3] = checksum; //7-bit checksum

    motor_uart_addToQueue(&MOTOR_UART_Queue, temp);
}
#endif
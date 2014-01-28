/********************************************************
 *   File Name: LED_SPI_ISR.c
 *
 *   Description:
 *              source file for the LED SPI ISR
 *
 *
 *********************************************************/

#include "LED_SPI_ISR.h"

/*************************************************************************
 Variables
 ************************************************************************/
LED_SPI_QUEUE LED_SPI_Queue;
boolean LED_SPI_is_idle;

/********************************************************
 *   Function Name: led_spi_setup()
 *
 *   Description: Initializes the SPI device for LEDs
 *
 *
 *********************************************************/
 void led_spi_setup(void) {

}

 /********************************************************
 *   Function Name: led_spi_setup_begin()
 *
 *   Description: Starts the LED_SPI ISR
 *
 *
 *********************************************************/
 inline void led_spi_setup_begin(void) {

}

/********************************************************
 *   Function Name: led_spi_InitializeQueue( LED_SPI_QUEUE* queue )
 *
 *   Description: Clears the queue and resets parameters
 *
 *
 *********************************************************/
void led_spi_InitializeQueue( LED_SPI_QUEUE* queue ) {
    memset(queue, 0, sizeof (LED_SPI_QUEUE));
}

/********************************************************
 *   Function Name: led_spi_addToQueue( LED_SPI_QUEUE* queue, LED_SPI_NODE new_node )
 *
 *   Description: Adds a node to the queue - Pass a node by reference
 *
 *
 *********************************************************/
int led_spi_addToQueue( LED_SPI_QUEUE* queue, LED_SPI_NODE new_node ) {

    if (queue->QueueEnd == queue->QueueStart && queue->QueueLength > 0) {
        return 1; //Error, would overwrite start of list
    }
    queue->DataBank[queue->QueueEnd] = new_node;
    queue->QueueLength++;
    if (queue->QueueEnd == LED_SPIQueueSize - 1) {
        queue->QueueEnd = 0;
    } else {
        queue->QueueEnd++;
    }

    return 0;
}

/********************************************************
 *   Function Name: led_spi_popNode( LED_SPI_QUEUE* queue, LED_SPI_NODE* return_node )
 *
 *   Description: Pulls the next node off the queue
 *
 *
 *********************************************************/
int led_spi_popNode( LED_SPI_QUEUE* queue, LED_SPI_NODE* return_node ) {

    if (queue->QueueLength == 0) {
        return 1; //Can't read from queue if empty
    }
    *return_node = (queue->DataBank[queue->QueueStart]); //Returns the Node
    if (queue->QueueStart == LED_SPIQueueSize - 1) {
        queue->QueueStart = 0;
    } else {
        queue->QueueStart++;
    }
    queue->QueueLength--;
    return 0;
}

/********************************************************
 *   Function Name: led_spi_Handler()
 *
 *   Description:
 *             ISR for the LED SPI module
 *
 *********************************************************/
void __ISR(_LED_SPI_VECTOR, IPL7AUTO) led_spi_Handler(void)
{
    LED_SPI_NODE current_node;

    INTDisableInterrupts();


        if (led_spi_popNode(&LED_SPI_Queue, &current_node)) {
            LED_SPI_is_idle = TRUE;
        } else {
            //transmit the data
        }

       //clear the interrupt flag

    INTEnableInterrupts();

}

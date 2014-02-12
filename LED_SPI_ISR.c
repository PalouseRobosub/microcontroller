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
     /*
1. If using interrupts:
    a) Clear the SPIxIF bit in the respective IFSx register.
    b) Select an interrupt mode using the SISEL<2:0> bits (SPIxSTAT<4:2>).
    c) Set the SPIxIE bit in the respective IECx register.
    d) Write the SPIxIP bits in the respective IPCx register.
2. When MSTEN (SPIxCON1<5>) = 1, write the desired settings to the SPIxCON1 and SPIxCON2 registers.
3. Clear the SPIROV bit (SPIxSTAT<6>).
4. Select Enhanced Buffer mode by setting the SPIBEN bit (SPIxCON2<0>).
5. Enable the SPIx operation by setting the SPIEN bit (SPIxSTAT<15>).
6. Write the data to be transmitted to the SPIxBUF register. The transmission (and reception) starts as soon as data is written to the SPIxBUF register.
*/

     LED_SPI_BRG = 0x04; //spi_clk = bus_clk/(2*(BRG+1)) so this sets clock to 1 Mhz

     LED_SPI_TXIF = 0; // clear the Tx interrupt flag
     LED_SPI_STXISEL = 0x01; //interrupt when the Tx buffer is empty
     LED_SPI_TXIE = 1; //enable the Tx interrupt
     LED_SPI_TX_INT_PRIORITY_set(7); //set the interrupt priority

     LED_SPI_MSTEN = 1; //enable master mode
     LED_SPI_ENHBUF = 1; //enable enhanced buffer mode
     LED_SPI_SPIROV = 0; //clear the receive overflow flag
     LED_SPI_CKP = 1;
     LED_SPI_CKE = 1;

     LED_SPI_ON = 1; //enable the SPI module


     LED_SPI_is_idle = TRUE;

}

 /********************************************************
 *   Function Name: led_spi_begin()
 *
 *   Description: Starts the LED_SPI ISR
 *
 *
 *********************************************************/
 inline void led_spi_begin(void) {

     LED_SPI_TXIE = 1;
     LED_SPI_TXIF = 1;

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
            LED_SPI_TXIE = 0; //clear the interrupt, so it doesn't keep firing
        } else {
            //transmit the data
            LED_SPI_BUF = current_node.data_G;
            LED_SPI_BUF = current_node.data_R;
            LED_SPI_BUF = current_node.data_B;
            

        }
       //clear the interrupt flag

    INTEnableInterrupts();

}

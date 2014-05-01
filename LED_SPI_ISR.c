/********************************************************
 *   File Name: LED_SPI_ISR.c
 *
 *   Description:
 *              source file for the LED SPI ISR
 *
 *
 *********************************************************/

/*************************************************************************
 System Includes
 ************************************************************************/
#include "system.h"
#include "LED_SPI_ISR.h"
#if defined (COMPILE_OLD_SUB) || defined (COMPILE_LED_BOARD)

/*************************************************************************
 Variables
 ************************************************************************/
LED_SPI_QUEUE LED_SPI_0_Queue, LED_SPI_1_Queue;
boolean LED_SPI_0_is_idle, LED_SPI_1_is_idle;

/********************************************************
 *   Function Name: led_spi_setup()
 *
 *   Description: Initializes the SPI device for LEDs
 *
 *
 *********************************************************/
void led_spi_0_setup(void) {
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

    LED_SPI_0_BRG = LED_SPI_0_CLK_DIV; //spi_clk = bus_clk/(2*(BRG+1)) so this sets clock to 1 Mhz

    LED_SPI_0_TXIF = 0; // clear the Tx interrupt flag
    LED_SPI_0_STXISEL = 0x01; //interrupt when the Tx buffer is empty
    LED_SPI_0_TXIE = 1; //enable the Tx interrupt
    LED_SPI_0_TX_INT_PRIORITY_set(7); //set the interrupt priority

    LED_SPI_0_MSTEN = 1; //enable master mode
    LED_SPI_0_ENHBUF = 1; //enable enhanced buffer mode
    LED_SPI_0_SPIROV = 0; //clear the receive overflow flag
    LED_SPI_0_CKP = 1;
    LED_SPI_0_CKE = 1;

    LED_SPI_0_ON = 1; //enable the SPI module


    LED_SPI_0_is_idle = TRUE;

}

#if defined (COMPILE_LED_BOARD)
void led_spi_1_setup(void) {
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

    LED_SPI_1_BRG = LED_SPI_1_CLK_DIV; //spi_clk = bus_clk/(2*(BRG+1)) so this sets clock to 1 Mhz

    LED_SPI_1_TXIF = 0; // clear the Tx interrupt flag
    LED_SPI_1_STXISEL = 0x01; //interrupt when the Tx buffer is empty
    LED_SPI_1_TXIE = 1; //enable the Tx interrupt
    LED_SPI_1_TX_INT_PRIORITY_set(7); //set the interrupt priority

    LED_SPI_1_MSTEN = 1; //enable master mode
    LED_SPI_1_ENHBUF = 1; //enable enhanced buffer mode
    LED_SPI_1_SPIROV = 0; //clear the receive overflow flag
    LED_SPI_1_CKP = 1;
    LED_SPI_1_CKE = 1;

    LED_SPI_1_ON = 1; //enable the SPI module


    LED_SPI_1_is_idle = TRUE;

}
#endif

/********************************************************
 *   Function Name: led_spi_begin()
 *
 *   Description: Starts the LED_SPI ISR
 *
 *
 *********************************************************/
inline void led_spi_0_begin(void) {

    LED_SPI_0_TXIE = 1;
    LED_SPI_0_TXIF = 1;

}

#if defined (COMPILE_LED_BOARD)
inline void led_spi_1_begin(void) {

    LED_SPI_1_TXIE = 1;
    LED_SPI_1_TXIF = 1;

}
#endif

/********************************************************
 *   Function Name: led_spi_InitializeQueue( LED_SPI_QUEUE* queue )
 *
 *   Description: Clears the queue and resets parameters
 *
 *
 *********************************************************/
void led_spi_InitializeQueue(LED_SPI_QUEUE* queue) {
    memset(queue, 0, sizeof (LED_SPI_QUEUE));
}

/********************************************************
 *   Function Name: led_spi_addToQueue( LED_SPI_QUEUE* queue, LED_SPI_NODE new_node )
 *
 *   Description: Adds a node to the queue - Pass a node by reference
 *
 *
 *********************************************************/
int led_spi_addToQueue(LED_SPI_QUEUE* queue, LED_SPI_NODE new_node) {

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
int led_spi_popNode(LED_SPI_QUEUE* queue, LED_SPI_NODE* return_node) {

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
void __ISR(_LED_SPI_0_VECTOR, IPL7AUTO) led_spi_0_Handler(void) {
    LED_SPI_NODE current_node;

    INTDisableInterrupts();


    if (led_spi_popNode(&LED_SPI_0_Queue, &current_node)) {
        LED_SPI_0_is_idle = TRUE;
        LED_SPI_0_TXIE = 0; //clear the interrupt, so it doesn't keep firing
    } else {
        LED_SPI_0_is_idle = FALSE;
        //transmit the data
        LED_SPI_0_BUF = current_node.data_G;
        LED_SPI_0_BUF = current_node.data_R;
        LED_SPI_0_BUF = current_node.data_B;


    }
    //clear the interrupt flag
    LED_SPI_0_TXIF = 0;

    INTEnableInterrupts();

}

#if defined (COMPILE_LED_BOARD)
void __ISR(_LED_SPI_1_VECTOR, IPL7AUTO) led_spi_1_Handler(void) {
    LED_SPI_NODE current_node;

    INTDisableInterrupts();


    if (led_spi_popNode(&LED_SPI_1_Queue, &current_node)) {
        LED_SPI_1_is_idle = TRUE;
        LED_SPI_1_TXIE = 0; //clear the interrupt, so it doesn't keep firing
    } else {
        LED_SPI_1_is_idle = FALSE;
        //transmit the data
        LED_SPI_1_BUF = current_node.data_G;
        LED_SPI_1_BUF = current_node.data_R;
        LED_SPI_1_BUF = current_node.data_B;


    }
    //clear the interrupt flag
    LED_SPI_1_TXIF = 0;

    INTEnableInterrupts();

}
#endif

/********************************************************
 *   Function Name: led_spi_load_pattern( uint8 pattern )
 *
 *   Description: Loads a pattern to the SPI LED write buffer
 *
 *
 *********************************************************/
void led_spi_load_pattern(uint8 pattern, LED_SPI_QUEUE* queue) {
    int i;
    LED_SPI_NODE temp, temp0;
    int count = 150;
    int bright = 127;

    //send initial zero packet to prep the strip
    temp0.data_G = 0x00;
    temp0.data_R = 0x00;
    temp0.data_B = 0x00;
    led_spi_addToQueue(queue, temp0);



    switch (pattern) {
        case LED_PATTERN_OFF:
            temp.data_G = SET_LED(0);
            temp.data_R = SET_LED(0);
            temp.data_B = SET_LED(0);
            for (i = 1; i < count; i = i + 1)
                led_spi_addToQueue(queue, temp);

            break;

        case LED_PATTERN_ALL_BLUE:
            temp.data_G = SET_LED(0);
            temp.data_R = SET_LED(0);
            temp.data_B = SET_LED(bright);
            for (i = 1; i < count; i = i + 1)
                led_spi_addToQueue(queue, temp);

            break;

        case LED_PATTERN_ALL_GREEN:
            temp.data_G = SET_LED(bright);
            temp.data_R = SET_LED(0);
            temp.data_B = SET_LED(0);
            for (i = 1; i < count; i = i + 1)
                led_spi_addToQueue(queue, temp);

            break;

        case LED_PATTERN_ALL_RED:
            temp.data_G = SET_LED(0);
            temp.data_R = SET_LED(bright);
            temp.data_B = SET_LED(0);
            for (i = 1; i < count; i = i + 1)
                led_spi_addToQueue(queue, temp);
            break;

        case LED_PATTERN_ALL_ORANGE:
            temp.data_G = SET_LED(7);
            temp.data_R = SET_LED(50);
            temp.data_B = 0x80;
            for (i = 1; i < count; i = i + 1)
                led_spi_addToQueue(queue, temp);
            break;

        case LED_PATTERN_ALL_PURPLE:
            temp.data_G = SET_LED(0);
            temp.data_R = SET_LED(bright);
            temp.data_B = SET_LED(bright);
            for (i = 1; i < count; i = i + 1)
                led_spi_addToQueue(queue, temp);
            break;

        case LED_PATTERN_RBB:
            temp.data_G = SET_LED(0);
            temp.data_R = SET_LED(50);
            temp.data_B = SET_LED(0);

            temp0.data_G = SET_LED(0);
            temp0.data_R = SET_LED(0);
            temp0.data_B = SET_LED(50);

            for (i = 1; i < count/3; i = i + 1) {
                led_spi_addToQueue(queue, temp);
                led_spi_addToQueue(queue, temp0);
                led_spi_addToQueue(queue, temp0);
            }
            break;

        case LED_PATTERN_BRB:
            temp.data_G = SET_LED(0);
            temp.data_R = SET_LED(50);
            temp.data_B = SET_LED(0);

            temp0.data_G = SET_LED(0);
            temp0.data_R = SET_LED(0);
            temp0.data_B = SET_LED(50);

            for (i = 1; i < count/3; i = i + 1) {
                led_spi_addToQueue(queue, temp0);
                led_spi_addToQueue(queue, temp);
                led_spi_addToQueue(queue, temp0);
            }
            break;

        case LED_PATTERN_BBR:
            temp.data_G = SET_LED(0);
            temp.data_R = SET_LED(50);
            temp.data_B = SET_LED(0);

            temp0.data_G = SET_LED(0);
            temp0.data_R = SET_LED(0);
            temp0.data_B = SET_LED(50);

            for (i = 1; i < count/3; i = i + 1) {
                led_spi_addToQueue(queue, temp0);
                led_spi_addToQueue(queue, temp0);
                led_spi_addToQueue(queue, temp);
            }
            break;

        default: //default pattern is turn white
            temp.data_G = SET_LED(bright);
            temp.data_R = SET_LED(bright);
            temp.data_B = SET_LED(bright);
            for (i = 1; i < count; i = i + 1)
                led_spi_addToQueue(queue, temp);
            break;

    }
}
#endif

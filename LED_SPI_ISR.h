/********************************************************
 *   File Name: LED_SPI_ISR.h
 *
 *   Description:
 *              header file for the LED SPI ISR
 *
 *
 *********************************************************/


#ifndef LED_SPI_ISR_H
#define	LED_SPI_ISR_H

/*************************************************************************
 System Includes
 ************************************************************************/

/*************************************************************************
 Constants
 ************************************************************************/
#define LED_SPIQueueSize 64

/*************************************************************************
 Macros
 ************************************************************************/

/*************************************************************************
 Enums
 ************************************************************************/
typedef enum //LED Patterns
{
    LED_PATTERN_ALL_BLUE,  //0
    LED_PATTERN_ALL_GREEN, //1
    LED_PATTERN_ALL_RED,   //2
    LED_PATTERN_ALL_WHITE, //3
    LED_PATTERN_RBB ,      //4
    LED_PATTERN_BRB,       //5
    LED_PATTERN_BBR,       //6
    LED_PATTERN_RWR       //7
    
} LED_PATTERN;

/*************************************************************************
 Structure Definitions
 ************************************************************************/


typedef struct led_spi_node
{
    uint8 data_G;
    uint8 data_R;
    uint8 data_B;

}LED_SPI_NODE;

typedef struct led_spi_queue
{
    LED_SPI_NODE DataBank[LED_SPIQueueSize]; //Array of Nodes of the #defined size QueueSize
    uint8 QueueStart; //Location of oldest node
    uint8 QueueEnd; //Location of next free slot
    uint8 QueueLength; //Length of queue
}LED_SPI_QUEUE;



/*************************************************************************
 Variables
 ************************************************************************/

/*************************************************************************
 Function Declarations
 ************************************************************************/
/********************************************************
 *   Function Name: led_spi_setup()
 *
 *   Description: Initializes the SPI device for LEDs
 *
 *
 *********************************************************/
 void led_spi_setup(void);

 /********************************************************
 *   Function Name: led_spi_begin()
 *
 *   Description: Starts the LED_SPI ISR
 *
 *
 *********************************************************/
 inline void led_spi_begin(void);

/********************************************************
 *   Function Name: led_spi_InitializeQueue( LED_SPI_QUEUE* queue )
 *
 *   Description: Clears the queue and resets parameters
 *
 *
 *********************************************************/
void led_spi_InitializeQueue( LED_SPI_QUEUE* queue );

/********************************************************
 *   Function Name: led_spi_addToQueue( LED_SPI_QUEUE* queue, LED_SPI_NODE new_node )
 *
 *   Description: Adds a node to the queue - Pass a node by reference
 *
 *
 *********************************************************/
int led_spi_addToQueue( LED_SPI_QUEUE* queue, LED_SPI_NODE new_node );

/********************************************************
 *   Function Name: led_spi_popNode( LED_SPI_QUEUE* queue, LED_SPI_NODE* return_node )
 *
 *   Description: Pulls the next node off the queue
 *
 *
 *********************************************************/
int led_spi_popNode( LED_SPI_QUEUE* queue, LED_SPI_NODE* return_node );

/********************************************************
 *   Function Name: led_spi_write_pattern( uint8 pattern )
 *
 *   Description: writes a pattern to the LEDs
 *
 *
 *********************************************************/
void led_spi_write_pattern( uint8 pattern );


#endif	/* LED_SPI_ISR_H */


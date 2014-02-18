/********************************************************
 *   File Name: ADC_ISR.h
 *
 *   Description:
 *              header file for the ADC ISR
 *
 *
 *********************************************************/

#ifndef ADC_ISR_H
#define	ADC_ISR_H

/*************************************************************************
 System Includes
 ************************************************************************/
#include "Sensors.h"

/*************************************************************************
 Typedefs
 ************************************************************************/

/*************************************************************************
 Constants
 ************************************************************************/
#define ADCQueueSize 16

/*************************************************************************
 Structure Definitions
 ************************************************************************/
typedef struct adc_node
{
    SENSOR_ID sensor_id;
    uint8 adc_channel; //device to read from

}ADC_Node;

typedef struct adc_queue
{
    ADC_Node DataBank[ADCQueueSize]; //Array of Nodes of the #defined size QueueSize
    uint8 QueueStart; //Location of oldest node
    uint8 QueueEnd; //Location of next free slot
    uint8 QueueLength; //Length of queue
}ADC_Queue;

/*************************************************************************
 Enums
 ************************************************************************/
typedef enum //ADC Channels
{
    ADC_CH_0,
    ADC_CH_1,
    ADC_CH_2,
    ADC_CH_3,
    ADC_CH_4,
    ADC_CH_5,
    ADC_CH_6,
    ADC_CH_7,
    ADC_CH_8,
    ADC_CH_9,
    ADC_CH_10,
    ADC_CH_11,
    ADC_CH_12,
    ADC_CH_13,
    ADC_CH_14,
    ADC_CH_15,

} ADC_CH;

/*************************************************************************
 Variables
 ************************************************************************/

/*************************************************************************
 Function Declarations
 ************************************************************************/

 /********************************************************
 *   Function Name:
 *
 *   Description:
 *
 *
 *********************************************************/

 /********************************************************
 *   Function Name:
 *
 *   Description:
 *
 *
 *********************************************************/
void adc_setup(void);

 /********************************************************
 *   Function Name:
 *
 *   Description:
 *
 *
 *********************************************************/
inline void adc_begin(void);

 /********************************************************
 *   Function Name:
 *
 *   Description:
 *
 *
 *********************************************************/
void ADC_InitializeQueue( ADC_Queue* queue );

 /********************************************************
 *   Function Name:
 *
 *   Description:
 *
 *
 *********************************************************/
int ADC_addToQueue( ADC_Queue* queue, ADC_Node new_node );

 /********************************************************
 *   Function Name:
 *
 *   Description:
 *
 *
 *********************************************************/
int ADC_popNode( ADC_Queue* queue, ADC_Node* return_node );

 /********************************************************
 *   Function Name:
 *
 *   Description:
 *
 *
 *********************************************************/

#endif	/* ADC_ISR_H */


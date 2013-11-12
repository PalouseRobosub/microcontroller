/********************************************************
 *   File Name: ADC_ISR.c
 *
 *   Description:
 *              source code file for the ADC ISR
 *
 *
 *********************************************************/
#include "system.h"
#include "functions.h"
#include "ADC_ISR.h"
#include "Sensors.h"
#include "comm_UART_ISR.h"


/*************************************************************************
 Variables
 ************************************************************************/
ADC_Queue ADC_queue;
boolean ADC_is_idle;
boolean ADC_initial;

/********************************************************
 *   Function Name: adc_setup()
 *
 *   Description:
 *
 *
 *********************************************************/
void adc_setup(void)
{
    //configure AD1PCFG
    AD1PCFG = 0x00000000;

    //Select the analog inputs to the ADC multiplexers in AD1CHS
    AD1CHSbits.CH0NA = 0; //set the negative input for channel A to internal ground


    TRISAbits.TRISA0 = 1; //make sure the pin is an input (not an output)
    TRISAbits.TRISA1 = 1;

    //Select the data format with FORM<2:0> (AD1CON1)
    AD1CON1bits.FORM = 0; //set the format to be simple 16 bit unsigned integer

    //Select the sample clock source using SSRC<2:0> (AD1CON1)
    AD1CON1bits.SSRC = 0b111; //set the clock source to be internal counter

    //Select the voltage reference source using VCFG<2:0> (AD1CON2)
    AD1CON2bits.VCFG = 000; //set the positve and negative references to be Vdd and Vss

    //Select the Scan mode using CSCNA (AD1CON2)
    AD1CON2bits.CSCNA = 0; //do not scan inputs

    //Set the number of conversions per interrupt SMP<3:0> (AD1CON2)
    AD1CON2bits.SMPI = 0; //interrupt after every conversion

    //Set Buffer Fill mode using BUFM (AD1CON2)
    AD1CON2bits.BUFM = 0; //use single 16 word buffer (don't really care, we aren't using the buffer)

    //Select the MUX to be connected to the ADC in ALTS AD1CON2
    AD1CON2bits.ALTS = 0; //always use mux A

    //Select the ADC clock source using ADRC (AD1CON3)
    AD1CON3bits.ADRC = 1; //use internal ADC clock

    //Select the sample time using SAMC<4:0> (AD1CON3)
    AD1CON3bits.SAMC = 15; //sample for 15 ADC clock cycles

    //Select the ADC clock prescaler using ADCS<7:0> (AD1CON3)
    //doesn't matter, we are using the internal oscillator




    //Turn the ADC module on using AD1CON1
    AD1CON1bits.ADON = 1;

    /*
     To configure ADC interrupt (if required):
a) Clear the AD1IF bit (IFS1<1>) (see 17.7).
b) Select ADC interrupt priority AD1IP<2:0> (IPC<28:26>) and subpriority AD1IS<1:0>
(IPC<24:24>) if interrupts are to be used (see 17.7).
15.Start the conversion sequence by initiating sampling (see 17.4.15).
     */

    ADC_IF = 0; //clear the interrupt flag
    ADC_INT_PRIORITY_set(7); //set the priority of the interrupt to 7
    ADC_INT_set(1); //enable the interrupt
    ADC_is_idle = TRUE;

}

 /********************************************************
 *   Function Name: adc_begin()
 *
 *   Description: Starts an ADC read
 *
 *
 *********************************************************/
 inline void adc_begin(void)
 {
     ADC_initial = TRUE; //say we are starting up the ADC
     ADC_IF = 1; //set the interrupt flag
 }

/********************************************************
 *   Function Name: ADC_Handler()
 *
 *   Description:
 *
 *
 *********************************************************/
void __ISR(_ADC_VECTOR, IPL7AUTO) ADC_Handler(void)
{
    uint16 adc_value;
    static ADC_Node current_node;

    INTDisableInterrupts();
    

    if (ADC_initial == FALSE)
    {
        adc_value =  ADC1BUF0;

        comm_uart_CreateNode(current_node.sensor_id, (adc_value & 0xFF), ((adc_value & 0xFF00) >> 8) );
        //ADC_Load_UART( current_node.sensor_id, adc_value);
    }

    if (ADC_popNode(&ADC_queue, &current_node)) //load next node from the queue
         {
             ADC_is_idle = TRUE; //flag that the bus is idle (nothing in the send queue)
             ADC_initial = TRUE;
         }
         else
         {
            ADC_is_idle = FALSE; //flag that the bus is working now
            AD1CHSbits.CH0SA = current_node.adc_channel; //set the positive input to be channel AN0
            AD1CON1bits.SAMP = 1;
            ADC_initial = FALSE;
         }

    ADC_IF = 0; //clear the interrupt flag

    INTEnableInterrupts();

}

/********************************************************
 *   Function Name: ADC_Load_UART(SENSOR_ID sensor, uint16 adc_value)
 *
 *   Description: Packages the received data and puts it
 *                on the UART queue
 *
 *
 *********************************************************/
void ADC_Load_UART(SENSOR_ID sensor, uint16 adc_value)
{

    switch (sensor)
    {
        case ADC_DEPTH_0:
            comm_uart_CreateNode(ADC_DEPTH, (adc_value & 0xFF), (adc_value >> 4));
            break;

        case ADC_BATT_0:
            comm_uart_CreateNode(ADC_BATT, (adc_value & 0xFF), (adc_value >> 4));
            break;
    }

    return;
}

/********************************************************
 *   Function Name: ADC_InitializeQueue( ADC_Queue* queue )
 *
 *   Description: Clears the queue and resets parameters
 *
 *
 *********************************************************/
void ADC_InitializeQueue( ADC_Queue* queue )
{
    memset(queue, 0, sizeof(ADC_Queue));
}
/********************************************************
 *   Function Name: ADC_addToQueue( ADC_Queue* queue, ADC_Node new_node )
 *
 *   Description: Adds a node to the queue - Pass a node by reference
 *
 *
 *********************************************************/
int ADC_addToQueue( ADC_Queue* queue, ADC_Node new_node )
{
    if (queue->QueueEnd == queue->QueueStart && queue->QueueLength > 0)
    {
        return 1; //Error, would overwrite start of list
    }
    queue->DataBank[queue->QueueEnd] = new_node;
    queue->QueueLength++;
    if (queue->QueueEnd == ADCQueueSize-1)
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
 *   Function Name: ADC_popNode( ADC_Queue* queue, ADC_Node* return_node )
 *
 *   Description: Pulls the next node off the queue
 *
 *
 *********************************************************/
int ADC_popNode( ADC_Queue* queue, ADC_Node* return_node )
{
    if (queue->QueueLength == 0)
    {
        return 1; //Can't read from queue if empty
    }
    *return_node = (queue->DataBank[queue->QueueStart]); //Returns the Node
    if (queue->QueueStart == ADCQueueSize-1)
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
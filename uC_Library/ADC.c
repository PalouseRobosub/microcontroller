
#include "ADC.h"

//callback functions
void (*ADC_callback) (void);

//channel data structures
ADC_Data adc_data;

boolean ADC_startup;

ADC_Data* initialize_ADC(uint8 *work_buffer_ptr, uint8 work_buffer_size,
                         uint8 *result_buffer_ptr, uint8 result_buffer_size, void* callback) {

    //setup the rx and tx buffers
    adc_data.Results_queue = create_queue(result_buffer_ptr, result_buffer_size);
    adc_data.Work_queue = create_queue(work_buffer_ptr, work_buffer_size);

    ADC_callback = callback; //link the callback function

    adc_data.is_idle = TRUE; //set the ADC module to idling

    //Select the analog inputs to the ADC multiplexers in AD1CHS
    AD1CHSbits.CH0NA = 0; //set the negative input for channel A to internal ground


    //make sure the pin is an input (not an output)
    //how to do this for only certain pins?

    //set pins to be analog
    //ANSELA
    //how to do this for only certain pins?

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

    //setup interrupts
    IFS0bits.AD1IF = 0; //clear the interrupt flag
    IPC5bits.AD1IP = 7; //set the priority of the interrupt to 7
    IEC0bits.AD1IE = 1; //enable the interrupt

    return &adc_data;
}

int read_ADC(ADC_Channel channel, uint8 device_id, void* callback) {
    ADC_Node new_node;
    int status;

    status = enqueue(&(adc_data.Work_queue), (uint8*) & new_node, sizeof (new_node));

    //if the bus is idling, force-start it
    if (adc_data.is_idle) {
        IFS1bits.I2C1MIF = 1;
    }


    return status;
}

void __ISR(_ADC_VECTOR, IPL7AUTO) ADC_Handler(void) {

    asm volatile ("di"); //disable interrupts

     uint16 adc_value;
    static ADC_Node current_node;
    extern boolean COMM_UART_is_idle;

    INTDisableInterrupts();


    if (ADC_startup == FALSE) {
        adc_value = ADC1BUF0;

        comm_uart_CreateNode(current_node.sensor_id, (adc_value & 0xFF), ((adc_value & 0xFF00) >> 8));
        if (COMM_UART_is_idle) {
            comm_uart_begin();
        }
    }

    if (ADC_popNode(&ADC_queue, &current_node)) //load next node from the queue
    {
        ADC_is_idle = TRUE; //flag that the bus is idle (nothing in the send queue)
        ADC_startup = TRUE;
    } else {
        ADC_is_idle = FALSE; //flag that the bus is working now
        AD1CHSbits.CH0SA = current_node.adc_channel; //set the positive input to be channel AN0
        AD1CON1bits.SAMP = 1;
        ADC_startup = FALSE;
    }




    if (ADC_callback != NULL) {
        ADC_callback();
    }

    IFS0bits.AD1IF = 0; //clear the interrupt flag

    asm volatile ("ei"); //reenable interrupts
}

/* 
 * File:   ADC.h
 * Author: james
 *
 * Created on December 24, 2014, 5:16 PM
 */

#ifndef ADC_H
#define	ADC_H

#ifdef	__cplusplus
extern "C" {
#endif

    /*Include Statements*/
#include "System.h"
#include "Queue.h"


    /* Enums */
    typedef enum {
        ADC_CH_0,
        ADC_CH_1,
        ADC_CH_2,
        ADC_CH_3,
        ADC_CH_4,
        ADC_CH_5,
        ADC_CH_9 = 9,
        ADC_CH_10,
        ADC_CH_11
    } ADC_Channel;


    /*Object Defintions*/
    //object for references to the data queues

    typedef struct ADC_DATA {
        //queue of resulting ADC reads
        Queue Results_queue;
        //queued ADC reads to be done
        Queue Work_queue;
        //idle information
        boolean is_idle;
    } ADC_Data;


    //ADC data node

    typedef struct ADC_NODE {
        uint8 device_id; //a unique identifier for the device
        ADC_Channel channel; //ADC channel
        uint16 data; //location to store data
        void (*callback) (struct ADC_NODE); //callback function
    } ADC_Node;

    typedef struct ADC_CONFIG {
        uint16 channels;
        uint8 *work_buffer_ptr;
        uint work_buffer_size;
        uint8 *result_buffer_ptr;
        uint result_buffer_size;
        void* callback;
    }ADC_Config;


    /*Function Prototypes*/
    ADC_Data* initialize_ADC(ADC_Config config);

    //set up an ADC read
    int read_ADC(ADC_Channel channel, uint8 device_id, void* callback);

    //run this background process in the main while loop to
    //process the results of ADC samples
    int bg_process_ADC(void);

    //configures the necessary pins as analog pins
    void setup_ADC_pins(uint16 channels);



#ifdef	__cplusplus
}
#endif

#endif	/* ADC_H */


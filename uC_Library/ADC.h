/* 
 * File:   ADC.h
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
    /** Set of all possible ADC channels */
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
    typedef struct ADC_DATA {
        /** Queue of resulting ADC reads */
        Queue Results_queue;
        /** queued ADC reads to be done */
        Queue Work_queue;
        /** idle information */
        boolean is_idle;
    } /** object for references to the data queues */ ADC_Data;

    typedef struct ADC_NODE {
        /** a unique identifier for the device */
        uint8 device_id;
        /** ADC channel */
        ADC_Channel channel;
        /** location to store data */
        uint16 data;
        /** callback function */
        void (*callback) (struct ADC_NODE);
    } /** ADC data node */ADC_Node;

    typedef struct ADC_CONFIG {
        /** The bit field of the channels to configure */
        uint16 channels;
        /** Pointer to the ADC's work buffer */
        uint8 *work_buffer_ptr;
        /** The size of the ADC's work buffer */
        uint work_buffer_size;
        /** Pointer to the ADC's result buffer */
        uint8 *result_buffer_ptr;
        /** The size of the ADC's result buffer */
        uint result_buffer_size;
        /** The function to call when done sampling <br/> <strong>default:</strong> NULL*/
        void* callback;
    }/** The configuration struct for the ADC */ ADC_Config;

    /*Function Prototypes*/
    /**
     * Initializes the ADC
     * @param config configuration information for the ADC
     * @see ADC_Config
     * @return A pointer to the results and the work queue for the ADC
     */
    ADC_Data* initialize_ADC(ADC_Config config);

    /**
     * Sets up the ADC to read
     * @param channel The ADC channel to read from
     * @param device_id ID that describes the device/channel
     * @param callback Function to execute when the data is ready
     * @return An Error type
     * @see Error in "System.h"
     */
    int read_ADC(ADC_Channel channel, uint8 device_id, void* callback);
    
    /**
     * Run this background process in the main while loop to
     * process the results of ADC samples
     */
    void bg_process_ADC(void);

    /**
     * Configures the necessary pins as analog pins
     * @param channels Bit mask to define which ADC channels to setup
     *                 i.e. channels = (1 << ADC_CH_1) | (1 << ADC_CH_4);
     */
    void setup_ADC_pins(uint16 channels);



#ifdef	__cplusplus
}
#endif

#endif	/* ADC_H */


/* 
 * File:   SPI.h
 * Author: james
 *
 * Created on December 28, 2014, 10:45 AM
 */

#ifndef SPI_H
#define	SPI_H

#ifdef	__cplusplus
extern "C" {
#endif

    /*Include Statements*/
#include "System.h"
#include "Queue.h"

    /* Enums */
    typedef enum {
        SPI_CH_1,
        SPI_CH_2

    } SPI_Channel;


    /*Object Defintions*/
    //object for references to the data queues

    typedef struct SPI_DATA {
        //received data
        Queue Rx_queue;
        //data that needs to be sent
        Queue Tx_queue;
        //idle information
        boolean is_idle;
    }
    SPI_Data;



    SPI_Data* initialize_SPI(uint speed, uint pb_clk, SPI_Channel which_spi,
            uint8 *rx_buffer_ptr, uint8 rx_buffer_size,
            uint8 *tx_buffer_ptr, uint8 tx_buffer_size, boolean tx_en, boolean rx_en,
            void* rx_callback, void* tx_callback);


    int send_SPI(SPI_Channel channel, uint8 *data_ptr, uint8 data_size);

    int receive_SPI(SPI_Channel channel, uint8 data_size, uint8 *data_ptr);




#ifdef	__cplusplus
}
#endif

#endif	/* SPI_H */


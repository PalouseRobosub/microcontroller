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
    } SPI_Data;

    typedef struct SPI_CONFIG {
        uint speed;
        uint pb_clk;
        SPI_Channel which_spi;
        uint8 clk_edge;
        uint8 *tx_buffer_ptr;
        uint tx_buffer_size;
        void (*tx_callback);
        boolean tx_en;
        uint8 *rx_buffer_ptr;
        uint rx_buffer_size;
        void (*rx_callback);
        boolean rx_en;
    }SPI_Config;



    
    SPI_Data* initialize_SPI(SPI_Config config);

    int send_SPI(SPI_Channel channel, uint8 *data_ptr, uint data_size);

    //int receive_SPI(SPI_Channel channel, uint8 *data_ptr, uint data_size);




#ifdef	__cplusplus
}
#endif

#endif	/* SPI_H */


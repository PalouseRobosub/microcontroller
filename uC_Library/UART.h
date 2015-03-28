/* 
 * File:   UART.h
 * Author: Ryan Summers
 *
 * Created on December 17, 2014, 4:22 PM
 */

#ifndef UART_H
#define	UART_H

#ifdef	__cplusplus
extern "C" {
#endif

    /*Inclues*/
#include "System.h"
#include "Queue.h"

    /*Enum Defintions*/

    /*Constants*/
    #define UART_FIFO_SIZE 8

    /*Object Defintions*/

    typedef struct UART_DATA {
        //Rx queue
        Queue Rx_queue;
        //Tx queue
        Queue Tx_queue;
        //idle information
        boolean Tx_is_idle;
    } UART_Data;

    typedef enum {
        UART1_CH,
        UART2_CH
    } UART_Channel;

    typedef struct UART_CONFIG {
        uint speed;
        uint pb_clk;
        UART_Channel which_uart;
        uint8 *rx_buffer_ptr;
        uint8 rx_buffer_size;
        uint8 *tx_buffer_ptr;
        uint8 tx_buffer_size;
        void (*rx_callback);
        void (*tx_callback);
        boolean tx_en;
        boolean rx_en;
    } UART_Config;

    /*Function Prototypes*/
    UART_Data* initialize_UART(UART_Config config);
    Error send_UART(UART_Channel channel, uint8 data_size, uint8 *data_ptr);
    Error receive_UART(UART_Channel channel, uint8 data_size, uint8 *data_ptr);
#ifdef	__cplusplus
}
#endif

#endif	/* UART_H */


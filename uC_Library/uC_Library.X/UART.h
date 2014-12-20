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

    /*Object Defintions*/

    typedef struct UART_DATA {
        //Rx queue
        Queue Rx_queue;
        //Tx queue
        Queue Tx_queue;
        //Callback function
        void (*function_ptr)(void);
        //idle information
        boolean Tx_is_idle;
        boolean Rx_is_idle;

    } Uart_Data;
    
    typedef enum {
        UART0,
        UART1
    }Uart;

    /*Function Prototypes*/
    Uart_Data* initialize_UART(uint speed, Uart which_uart, uint8 *rx_buffer_ptr, uint8 rx_buffer_size,
        uint8 *tx_buffer_ptr, uint8 tx_buffer_size, boolean tx_en, boolean rx_en,
        void* rx_callback, void* tx_callback);
#ifdef	__cplusplus
}
#endif

#endif	/* UART_H */


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
        bool Tx_is_idle;
        bool Rx_is_idle;

    } Uart_Data;
    
    typedef enum {
        UART0,
        UART1
    }Uart;

    /*Function Prototypes*/
    Uart_Data initialize_UART(uint speed, uint8 *rx_ptr, uint8 rx_size, uint8 *tx_ptr, uint8 tx_size, bool tx_en, bool rx_en);

#ifdef	__cplusplus
}
#endif

#endif	/* UART_H */


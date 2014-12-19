
#include "UART.h"

Uart_Data initialize_UART(uint speed, Uart which_uart, uint8 *rx_buffer_ptr, uint8 rx_buffer_size, uint8 *tx_buffer_ptr, uint8 tx_buffer_size, boolean tx_en, boolean rx_en) {
        //initialize our uart
    Uart_Data new_uart;

    //setup the rx and tx buffers
    new_uart.Rx_queue = create_buffer()
    new_uart.Tx_queue = create_buffer()

    switch (which_uart)
    {
        

    }

    




    }
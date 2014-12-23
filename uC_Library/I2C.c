
#include "I2C.h"

//callback functions
void (*I2C_1_callback) (void);
void (*I2C_2_callback) (void);


I2C_Data i2c1;
I2C_Data i2c2;

I2C_Data* initialize_I2C(uint pb_clk, I2C_Channel channel, uint8 *rx_buffer_ptr, uint8 rx_buffer_size,
        uint8 *tx_buffer_ptr, uint8 tx_buffer_size, void* callback) {

    switch (channel) {
        case I2C1:
            I2C1BRG = 1/(2*I2C_SPEED)*pb_clk - 2;  //calculate the proper divider

            //setup the rx and tx buffers
            i2c1.Rx_queue = create_queue(rx_buffer_ptr, rx_buffer_size);
            i2c1.Tx_queue = create_queue(tx_buffer_ptr, tx_buffer_size);

            I2C_1_callback = callback; //link the callback function

            i2c1.Tx_is_idle = TRUE;

            I2C1CONbits.ON = 0b1; //enable the I2C module'
            return &i2c1;
            break;

        default:
            //some sort of error handling?
            break;
    }

    return NULL;
}

void __ISR(_I2C_1_VECTOR, IPL7AUTO) I2C_1_Handler(void) {
    asm volatile ("di"); //disable interrupt


    asm volatile ("ei"); //reenable interrupts
}

void __ISR(_I2C_2_VECTOR, IPL7AUTO) I2C_2_Handler(void) {
    asm volatile ("di"); //disable interrupt


    asm volatile ("ei"); //reenable interrupts
}

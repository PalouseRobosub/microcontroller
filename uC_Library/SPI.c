
#include "SPI.h"

//callback functions
void (*spi_1_tx_callback) (void);
void (*spi_1_rx_callback) (void);

void (*spi_2_tx_callback) (void);
void (*spi_2_rx_callback) (void);


SPI_Data spi_1;
SPI_Data spi_2;

SPI_Data* initialize_SPI(uint speed, uint pb_clk, SPI_Channel which_spi,
                         uint8 *rx_buffer_ptr, uint8 rx_buffer_size,
                         uint8 *tx_buffer_ptr, uint8 tx_buffer_size, boolean tx_en, boolean rx_en,
                         void* rx_callback, void* tx_callback) {

    /*
    1. If using interrupts:
        a) Clear the SPIxIF bit in the respective IFSx register.
        b) Select an interrupt mode using the SISEL<2:0> bits (SPIxSTAT<4:2>).
        c) Set the SPIxIE bit in the respective IECx register.
        d) Write the SPIxIP bits in the respective IPCx register.
    2. When MSTEN (SPIxCON1<5>) = 1, write the desired settings to the SPIxCON1 and SPIxCON2 registers.
    3. Clear the SPIROV bit (SPIxSTAT<6>).
    4. Select Enhanced Buffer mode by setting the SPIBEN bit (SPIxCON2<0>).
    5. Enable the SPIx operation by setting the SPIEN bit (SPIxSTAT<15>).
    6. Write the data to be transmitted to the SPIxBUF register. The transmission (and reception) starts as soon as data is written to the SPIxBUF register.
     */

    switch (which_spi) {
        case SPI_CH_1:

            //setup the rx and tx buffers
            spi_1.Rx_queue = create_queue(rx_buffer_ptr, rx_buffer_size);
            spi_1.Tx_queue = create_queue(tx_buffer_ptr, tx_buffer_size);

            spi_1_tx_callback = tx_callback;
            spi_1_rx_callback = rx_callback;

            SPI1BRG = pb_clk / (2 * speed) - 1; //set divisor to match desired speed

            IFS1bits.SPI1TXIF = 0; // clear the Tx interrupt flag
            SPI1CONbits.STXISEL = 0x01; //interrupt when the Tx buffer is empty
            IEC1bits.SPI1TXIE = (tx_en & 0b1); //enable the Tx interrupt
            IPC7bits.SPI1IP = 7; //set the interrupt priority

            SPI1CONbits.MSTEN = 1; //enable master mode
            SPI1CONbits.ENHBUF = 1; //enable enhanced buffer mode
            SPI1STATbits.SPIROV = 0; //clear the receive overflow flag
            SPI1CONbits.CKP = 1;
            SPI1CONbits.CKE = 1;

            SPI1CONbits.ON = 1; //enable the SPI module

            spi_1.is_idle = TRUE;

            return &spi_1;
            break;

        case SPI_CH_2:

            //setup the rx and tx buffers
            spi_2.Rx_queue = create_queue(rx_buffer_ptr, rx_buffer_size);
            spi_2.Tx_queue = create_queue(tx_buffer_ptr, tx_buffer_size);

            spi_2_tx_callback = tx_callback;
            spi_2_rx_callback = rx_callback;

            SPI2BRG = pb_clk / (2 * speed) - 1; //set divisor to match desired speed

            IFS1bits.SPI2TXIF = 0; // clear the Tx interrupt flag
            SPI2CONbits.STXISEL = 0x01; //interrupt when the Tx buffer is empty
            IEC1bits.SPI2TXIE = (tx_en & 0b1); //enable the Tx interrupt
            IPC9bits.SPI2IP = 7; //set the interrupt priority

            SPI2CONbits.MSTEN = 1; //enable master mode
            SPI2CONbits.ENHBUF = 1; //enable enhanced buffer mode
            SPI2STATbits.SPIROV = 0; //clear the receive overflow flag
            SPI2CONbits.CKP = 1;
            SPI2CONbits.CKE = 1;

            SPI2CONbits.ON = 1; //enable the SPI module

            spi_2.is_idle = TRUE;

            return &spi_2;
            break;

        default:
            //some sort of error handling?
            break;
    }

    return NULL;
}

int send_SPI(SPI_Channel channel, uint8 *data_ptr, uint8 data_size) {
    int status;
    //we need to place the provided data onto the Tx queue
    switch (channel) {
        case SPI_CH_1:
            status = enqueue(&(spi_1.Tx_queue), data_ptr, data_size);
            if (spi_1.is_idle) { //if the tx is idle, force-start it
                IFS1bits.SPI1TXIF = 1;
            }
            break;
        case SPI_CH_2:
            status = enqueue(&(spi_2.Tx_queue), data_ptr, data_size);
            if (spi_2.is_idle) { ////if the tx is idle, force-start it
                IFS1bits.SPI2TXIF = 1;
            }
            break;
        default:
            status = 1; //return failure
            break;

    }
    return status;
}

int receive_SPI(SPI_Channel channel, uint8 data_size, uint8 *data_ptr) {
    int status;
    //we need to read the specified data from the rx queue
    switch (channel) {
        case SPI_CH_1:
            status = dequeue(&(spi_1.Rx_queue), data_ptr, data_size);
            break;
        case SPI_CH_2:
            status = dequeue(&(spi_2.Rx_queue), data_ptr, data_size);
            break;
        default:
            status = 1; //return failure
            break;
    }
    return status; //return success
}

void __ISR(_SPI_1_VECTOR, IPL7AUTO) SPI_1_Handler(void) {
    uint8 received, transmit;
    asm volatile ("di"); //disable interrupt

    if (IFS1bits.SPI1TXIF) { //if the interrupt flag of TX is set

        //if the transmit queue is empty
        if (spi_1.Tx_queue.numStored == 0) {
            //our buffer is empty
            //we need to disable the interrupts
            IEC1bits.SPI1TXIE = 0;
            //and set the queue to idle
            spi_1.is_idle = 1;

        } else {
            //we have data to transmit - pop that data off the queue
            //store popped data into the transmit registry
            //while there is data in queue AND the transmit buffer is not full
            while (!dequeue(&(spi_1.Tx_queue), &transmit, 1) && !SPI1STATbits.SPITBF) { 
                //write the data to the buffer
                SPI1BUF = transmit;
            } //write data until the queue is empty or the registry is full

            if (spi_1_tx_callback != NULL) {
                spi_1_tx_callback(); //call additional ISR functionality
            }
            //now clear the interrupt flag
            IFS1bits.SPI1TXIF = 0;
        }
    }

    asm volatile ("ei"); //reenable interrupts
}

void __ISR(_SPI_2_VECTOR, IPL7AUTO) SPI_2_Handler(void) {
    uint8 received, transmit;
    asm volatile ("di"); //disable interrupt

        if (IFS1bits.SPI2TXIF) { //if the interrupt flag of TX is set

        //if the transmit queue is empty
        if (spi_2.Tx_queue.numStored == 0) {
            //our buffer is empty
            //we need to disable the interrupts
            IEC1bits.SPI2TXIE = 0;
            //and set the queue to idle
            spi_2.is_idle = 1;

        } else {
            //we have data to transmit - pop that data off the queue
            //store popped data into the transmit registry
            //while there is data in queue AND the transmit buffer is not full
            while (!dequeue(&(spi_2.Tx_queue), &transmit, 1) && !SPI2STATbits.SPITBF) {
                //write the data to the buffer
                SPI2BUF = transmit;
            } //write data until the queue is empty or the registry is full

            if (spi_2_tx_callback != NULL) {
                spi_2_tx_callback(); //call additional ISR functionality
            }
            //now clear the interrupt flag
            IFS1bits.SPI2TXIF = 0;
        }
    }

    asm volatile ("ei"); //reenable interrupts
}

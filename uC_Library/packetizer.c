#include "packetizer.h"
#include "UART.h"

Packetizer_Data UART1_channel;
Packetizer_Data UART2_channel;

void initialize_packetizer(Data_Channel which_channel, uint8 control_byte, void* callback) {

    switch (which_channel) {
        case PACKET_UART1:
            UART1_channel.control_byte = control_byte;
            UART1_channel.receive_callback = callback;
            break;
        case PACKET_UART2:
            UART2_channel.control_byte = control_byte;
            UART2_channel.receive_callback = callback;
            break;
    }

}

void send_packet(Data_Channel which_channel, uint8* data, uint8 data_size) {
    uint interrupt_state;
    interrupt_state = __builtin_get_isr_state();
    __builtin_disable_interrupts();

    switch (which_channel) {
        case PACKET_UART1:
            send_UART(UART1, 1, &(UART1_channel.control_byte));
            send_UART(UART1, data_size, data);
            break;
        case PACKET_UART2:
            send_UART(UART2, 1, &(UART2_channel.control_byte));
            send_UART(UART2, data_size, data);
            break;
    }

    __builtin_set_isr_state(interrupt_state);
}

void packetizer_background_process(Data_Channel which_channel) {
    uint8 current_byte;
    uint8 status;

    void (*receive_callback) (uint8* data, uint8 data_size); //receive callback function for the channel
    uint8* control_byte; //control byte for the channel
    boolean* sync_lock;
    boolean* packet_received;
    uint8* received_bytes;
    uint16* received_index;
    uint8* packet_length;

    //load data from queues
    //form packets by getting data from the queue
    //if a packet is sucessfully formed, do the callback
    //packetizer_callback(packet);



    //get one byte and setup the variables
    //this is the only thing that is device-specific
    switch (which_channel) {
        case PACKET_UART1:
            status = receive_UART(UART1, 1, &current_byte);
            receive_callback = UART1_channel.receive_callback;
            control_byte = &UART1_channel.control_byte;
            sync_lock = &UART1_channel.sync_lock;
            packet_received = &UART1_channel.packet_received;
            received_bytes = UART1_channel.received_bytes; //array, so already pointer
            received_index = &UART1_channel.received_index;
            packet_length = &UART1_channel.packet_length;
            break;
        case PACKET_UART2:
            status = send_UART(UART2, 1, &current_byte);
            receive_callback = UART2_channel.receive_callback;
            control_byte = &UART2_channel.control_byte;
            sync_lock = &UART2_channel.sync_lock;
            packet_received = &UART2_channel.packet_received;
            received_bytes = UART2_channel.received_bytes; //array, so already pointer
            received_index = &UART2_channel.received_index;
            packet_length = &UART2_channel.packet_length;
            break;
    }

    if (status == 0) //if there were no errors we got a byte out of the buffer
    {
        if (*sync_lock) {
            switch (*received_index) {
                case 0: //first byte, should be control byte
                    *packet_received = FALSE;
                    ++(*received_index);
                    if (current_byte != *control_byte) {
                        *sync_lock = FALSE;
                        *received_index = 0;
                    }
                    break;
                case 1: //second byte, length byte
                    *packet_length = current_byte;
                    ++(*received_index);
                    break;

                default: //any other bytes are data bytes
                    received_bytes[*received_index - 2] = current_byte; //store the received data
                    if (*received_index == *packet_length) //we've received the entire packet
                    {
                        *packet_received = TRUE;
                        *received_index = 0;
                    }
                    break;
            }

        } else { //we're out of sync, need to regain sync
            switch (*received_index) {
                case 0:
                    if (current_byte == *control_byte) {
                        *received_index = 1;
                    }
                    break;
                case 1:
                    *packet_length = current_byte;
                    ++(*received_index);
                    break;
                default:
                    //absorb bytes until we have received the entire packet
                    //receive one more, check for control byte
                    //if control byte, then say we are in sync and prep to receive rest of packet
                    ++(*received_index);
                    if (*received_index == *packet_length+2) //maybe length+3?
                    {
                        if (current_byte == *control_byte) //we appear to be in sync
                        {
                            *sync_lock = TRUE;
                            *received_index = 1;
                        }
                    }
                    break;
            }
        }

        if (*packet_received) //if we've receive a packet, process it!
        {
            receive_callback(received_bytes, *packet_length);
        }


    } //else, some error, don't do anything





}

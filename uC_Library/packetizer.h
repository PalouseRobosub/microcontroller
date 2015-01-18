/* 
 * File:   packetizer.h
 * Author: james
 *
 * Created on January 17, 2015, 11:32 PM
 */

#ifndef PACKETIZER_H
#define	PACKETIZER_H

#ifdef	__cplusplus
extern "C" {
#endif

    /*Include Statements*/
#include "System.h"

    typedef enum {
        PACKET_UART1,
        PACKET_UART2
    } Data_Channel;

    typedef struct PACKETIZER_DATA {
       void (*receive_callback) (uint8* data, uint8 data_size); //receive callback function for the channel
       uint8 control_byte; //control byte for the channel
       boolean sync_lock;
       boolean packet_received;
       uint8 received_bytes[255];
       uint16 received_index;
       uint8 packet_length;
    } Packetizer_Data;


#ifdef	__cplusplus
}
#endif

#endif	/* PACKETIZER_H */


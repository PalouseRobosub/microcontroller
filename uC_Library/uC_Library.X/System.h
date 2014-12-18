/* 
 * File:   System.h
 * Author: Ryan Summers
 *
 * Created on December 17, 2014, 12:52 PM
 */

#ifndef SYSTEM_H
#define	SYSTEM_H

#ifdef	__cplusplus
extern "C" {
#endif


/* Includes */
#define _SUPPRESS_PLIB_WARNING
#include <peripheral/ports.h>

/* Type Defintions*/
    typedef unsigned int uint;
    typedef unsigned short int uint16;
    typedef unsigned char uint8;
    typedef signed char sint8;

    typedef enum {
        FALSE,
        TRUE
    } boolean;


/* Structure Definitions*/
    typedef struct QUEUE {
        uint8 *buffer; //pointer to the queue memory
        uint buffer_size; //size of the supplied buffer
        uint QueueStart; //location of first data point (start of queue)
        uint QueueEnd; //location of the last data point (end of queue)
        uint QueueLength; //amount of data within the queue
    } Queue;


/*Queue Functions*/
    Queue create_queue(uint8* buffer, uint buffer_size);
    int enqueue(Queue* queue, uint8* data, uint data_size);
    int dequeue(Queue* queue, uint8* output_data, uint data_size);

#ifdef	__cplusplus
}
#endif

#endif	/* SYSTEM_H */


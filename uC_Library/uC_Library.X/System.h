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


/* Includes*/
#define _SUPPRESS_PLIB_WARNING
#include <peripheral/ports.h>

/* Type Defintions*/
    typedef unsigned int uint;
    typedef unsigned short int uint16;
    typedef unsigned char uint8;
    typedef signed char sint8;
    typedef uint8 boolean;

/* Structure Definitions*/
    typedef struct QUEUE {
        uint8 *buffer; //pointer to the queue memory
        uint8 buffer_size; //size of the supplied buffer
        uint8 QueueStart; //location of first data point (start of queue)
        uint8 QueueEnd; //location of the last data point (end of queue)
        uint8 QueueLength; //amount of data within the queue
    } Queue;

#ifdef	__cplusplus
}
#endif

#endif	/* SYSTEM_H */


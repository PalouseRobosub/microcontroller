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


#ifdef	__cplusplus
}
#endif

#endif	/* SYSTEM_H */


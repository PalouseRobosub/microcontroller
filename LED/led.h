/* 
 * File:   led.h
 * Author: connor
 *
 * Created on May 30, 2015, 6:19 PM
 */

#ifndef LED_H
#define	LED_H

#ifdef	__cplusplus
extern "C" {
#endif


#define NUMLEDS 64
#define STRIPSIZE 32

typedef struct Pixel
{
	unsigned char green, red, blue;
} PIXEL;

typedef enum {
	manual,
	all,
	strip,
	dual,
	set
} ComCodes;


#ifdef	__cplusplus
}
#endif

#endif	/* LED_H */


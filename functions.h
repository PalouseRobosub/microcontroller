/* 
 * File:   functions.h
 * Author: James
 *
 * Created on September 21, 2013, 8:06 PM
 */

#ifndef FUNCTIONS_H
#define	FUNCTIONS_H

#include <peripheral/ports.h>

unsigned int read_buttons(void);
void write_leds(unsigned int);

void setup_I2C (void);


#endif	/* FUNCTIONS_H */


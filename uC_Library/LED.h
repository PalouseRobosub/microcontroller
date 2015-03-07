/* 
 * File:   LED.h
 * Author: Admin
 *
 * Created on 2015?03?03? ???, ??3:27
 */

#ifndef LED_H
#define	LED_H

#ifdef	__cplusplus
extern "C" {
#endif
#include "System.h"
#include "SPI.h"
    uint16
        numLEDs,    // Number of RGB LEDs in strip
        numBytes;   // Size of 'pixels' buffer below
    uint8
        *pixels,    // Holds LED color values (3 bytes each) + latch bytes
        clkpin, 
        datapin;     // Clock & data pin numbers
  void
    setPixelColor(uint16 n, uint c),
    show(void);
  uint
    Color(sint8, sint8, sint8);


#ifdef	__cplusplus
}
#endif

#endif	/* LED_H */


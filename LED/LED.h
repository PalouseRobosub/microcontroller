/* 
 * File:   LED.h
 * Author: Nicholas Yeung
 *
 * Created on 2015?03?03? ???, ??3:27
 */

#ifndef LED_H
#define	LED_H

#ifdef	__cplusplus
extern "C" {
#endif
#include "../uC_Library/System.h"
#include "../uC_Library/SPI.h"
#include "../uC_Library/packetizer.h"

    uint16
        numLEDs=64,    // Number of RGB LEDs in strip
        numBytes;   // Size of 'pixels' buffer below
    uint8
        clkpin,
        pixbuf[(numLEDs*3)+1],//size is number of LEDs *3 + 1 latch byte
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


/* 
 * File:   LED.c
 * Author: Admin
 *
 * Created on 2015?03?03? ???, ??3:27
 */

#include <stdio.h>
#include <stdlib.h>
#include "LED.h"

uint16 numPixels(void) {
  return numLEDs;
}

void show(void) {
  //uint8  *ptr = pixels;
  //uint16 i    = (32 * 3)+(63/32);
    uint8 *ptr = pixels;
    uint16 i;
    uint8 toggle = 0;
    if(toggle) {
        for(i=0; i < 32*3; i++)
        {
            ptr[i] = 0x80;
        }
        toggle = 0;
    }
    else {
        for(i=0; i < 32*3; i++)
        {
            ptr[i] = 0x00;
        }
        toggle = 1;
    }
  // This doesn't need to distinguish among individual pixel color
  // bytes vs. latch data, etc.  Everything is laid out in one big
  // flat buffer and issued the same regardless of purpose.
        send_SPI(SPI_CH_1,ptr,i);
}

// Convert separate R,G,B into combined 32-bit GRB color:
uint Color(sint8 r, sint8 g, sint8 b) {
  return ((uint)(g & 0xFF) << 16) |
         ((uint)(r & 0xFF) <<  8) |
                     b & 0xFF ;
}

// Set pixel color from 'packed' 32-bit GRB (not RGB) value:
void setPixelColor(uint16 n, uint c) {
  if(n < 32) { // Arrays are 0-indexed, thus NOT '<='
    uint8 *p = &pixels[n * 3];
    *p++ = (c >> 16) & 0xFF;
    *p++ = (c >>  8) & 0xFF;
    *p++ =  c    & 0xFF;
  }
}
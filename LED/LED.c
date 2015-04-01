/* 
 * File:   LED.c
 * Author: Nicholas Yeung
 *
 * Created on 2015?03?03? ???, ??3:27
 */
/*Need to change SPI to Falling edge
 so set SPI1CONbits.CKE to 0*/

#include <stdio.h>
#include <stdlib.h>
#include "LED.h"

uint16 numPixels(void) {
  return numLEDs;
}

void show(void) {
  uint8  *ptr = pixbuf;
  uint16 i    = (numLEDs*3)+1;
  int j=0;
  ptr[(numLEDs*3)]=0;
  while(i--)
  {
    send_SPI(SPI_CH_1,ptr,1);
    ptr++;
    for(j=0;j<5;j++)
    {
        asm volatile ("nop");
        asm volatile ("nop");
        asm volatile ("nop");
        asm volatile ("nop");
    }
  }
}
// Convert separate R,G,B into combined 32-bit GRB color:
uint Color(sint8 r, sint8 g, sint8 b) {
  return ((uint)(g | 0x80) << 16) |
         ((uint)(r | 0x80) <<  8) |
                     b | 0x80 ;
}
// Set pixel color from 'packed' 32-bit GRB (not RGB) value:
void setPixelColor(uint16 n, uint c) {
  if(n < numLEDs) { // Arrays are 0-indexed, thus NOT '<='
    uint8 *p = &pixels[n * 3];
    *p++ = (c >> 16) | 0x80;
    *p++ = (c >>  8) | 0x80;
    *p++ =  c    | 0x80;
  }
}
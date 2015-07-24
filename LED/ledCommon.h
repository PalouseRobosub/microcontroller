#ifndef LED_COMMON_H
#define	LED_COMMON_H

#define NUMLEDS 16
#define STRIPSIZE 8

typedef struct Pixel
{
	uint8 green, red, blue;
} PIXEL;

typedef enum {
	manual,
	all,
	strip,
	dual_all,
    dual_strip,
	set
} ComCodes;

typedef enum {
    forward_warp_shift,
    backward_warp_shift,
            clockwise_shift,
           counterclockwise_shift,
    toggle
}TimerMode;


#endif
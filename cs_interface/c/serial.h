#ifndef __SERIAL_H__
#define __SERIAL_H__


//Defines
#ifndef BAUD_RATE
#define BAUD_RATE B115200
//#define BAUD_RATE B19200
//#define BAUD_RATE B9600
#endif

//function definitions
int s_open(char *port);


#endif 

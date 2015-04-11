#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "serial.h"


int main(int argc, char**argv)
{
	int ser;
	int n,i;
	char str[128];
	int strlen;
	char port[64] = "/dev/ttyUSB0";


	if(argc >= 2)
	{
		strcpy(port, argv[1]);
	}


	strcpy(str, "blah\nyay\n");
	for (strlen=0; (str[strlen] != 0) &&  (strlen < sizeof(str)); ++strlen);

	ser = s_open(port);

	if (ser < 0)
	{
		printf("could not open serial port: %s\n", port);
		return 1;
	}
	else
	{
		printf("serial port opened: %s\n", port);
	}


	for (i=0; i < 256; ++i)
	{
		write(ser, &i, 1);
	}
/*	
	n = write(ser, str, strlen);
	if (n < 0)
		  fputs("write() of 4 bytes failed!\n", stderr);
	else
		printf("write succeeded, wrote %d bytes out of %d bytes\n", n, strlen);	
*/	
	return 0;
}


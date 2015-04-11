#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "serial.h"


int main(int argc, char**argv)
{
	int ser;
	int n,i;
	char port[64] = "/dev/ttyUSB0";


	if(argc >= 2)
	{
		strcpy(port, argv[1]);
	}



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


	printf ("reading bytes...\n");
	for (i=0; i < 256; ++i)
	{
		read(ser, &n, 1);
		printf("%x\n", n);
	}
	printf("done!\n");


	return 0;
}


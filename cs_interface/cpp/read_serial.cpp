#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "serial.h"


int main(int argc, char**argv)
{
	int i;
	char n;
	char port[64] = "/dev/ttyUSB0";

	if(argc >= 2)
	{
		strcpy(port, argv[1]);
	}
	Serial s(port);	

	printf("serial port opened: %s\n", port);

	printf ("reading bytes...\n");
	for (i=0; i < 256; ++i)
	{
		s.sread(&n, 1);
		printf("%hhx\n", n);
	}
	printf("done!\n");


	return 0;
}


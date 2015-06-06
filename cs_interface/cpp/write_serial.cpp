#include <string.h>
#include <stdio.h>

#include "serial.h"


int main(int argc, char**argv)
{
	int i;
	char port[64] = "/dev/ttyUSB0";

	if(argc >= 2)
	{
		strcpy(port, argv[1]);
	}
	Serial s(port);
		

	printf("serial port opened: %s\n", port);


	for (i=0; i < 3; ++i)
	{
		s.swrite((char*)&i, 1);
	}


	return 0;
}


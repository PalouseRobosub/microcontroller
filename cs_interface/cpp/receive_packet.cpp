#include <string.h>
#include <stdio.h>

#include "packetizer.h"


int main(int argc, char**argv)
{
	int i;
	char port[64] = "/dev/ttyUSB0";
	char msg[64];

	if(argc >= 2)
	{
		strcpy(port, argv[1]);
	}
	Packetizer p(port);

	printf("serial port opened: %s\n", port);


	
	while (1)
	{
		p.get(msg, (char*)&i);
		printf("received: %s\n", msg);
	}

	printf("done receiving!\n");

	return 0;
}


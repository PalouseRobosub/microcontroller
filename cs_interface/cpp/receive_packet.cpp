#include <string.h>
#include <stdio.h>

#include "packetizer.h"


int main(int argc, char**argv)
{
	int i, j;
	char port[64] = "/dev/ttyUSB0";
	char msg[64];
	int tmp;

	if(argc >= 2)
	{
		strcpy(port, argv[1]);
	}
	Packetizer p(port);

	printf("serial port opened: %s\n", port);


	
	while (1)
	{
		printf ("getting packet...\n");
		i = p.get(msg);
		if (i > 0)
		{
			printf("received: %d bytes\n", i);
			for(j=0; j < i; ++j)
			{
				tmp = msg[j];
				printf("%2x: '%c'\n", tmp, tmp);
			}
		}
	}

	printf("done receiving!\n");

	return 0;
}


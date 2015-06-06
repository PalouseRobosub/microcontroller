#include <string.h>
#include <stdio.h>

#include "packetizer.h"


int main(int argc, char**argv)
{
	int i;
	char port[64] = "/dev/ttyUSB0";
	char msg[3][32] = {"hey", "world", "test successful!"};

	if(argc >= 2)
	{
		strcpy(port, argv[1]);
	}
	Packetizer p(port);
		

	printf("serial port opened: %s\n", port);
	for (i=2; i < 3; ++i)
	{
		printf("sending \'%s\', length: %d\n", msg[i], strlen(msg[i])+1);
		p.send(msg[i], strlen(msg[i])+1);
	}
	/*
	for (i=0; i <= 0; ++i)
	{
		printf("sending %x\n", i);
		p.send("hello_world", sizeof();
	}
	*/

	printf("done sending!\n");

	return 0;
}


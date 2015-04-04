#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "serial.h"


int main(int argc, char**argv)
{
	int ser;
	int n;
	char str[128];
	int strlen;

	strcpy(str, "blah\nyay\n");
	for (strlen=0; (str[strlen] != 0) &&  (strlen < sizeof(str)); ++strlen);

	ser = s_open("/dev/ttyUSB0");

	if (ser < 0)
	{
		printf("could not open serial port in \n");
	}
	else
	{
		printf("serial port opened\n");
	}

	configure(ser);

	n = write(ser, str, strlen);
	if (n < 0)
		  fputs("write() of 4 bytes failed!\n", stderr);
	else
		printf("write succeeded, wrote %d bytes out of %d bytes\n", n, strlen);	
	
}




void configure(int fd) 
{
	struct termios options;

	tcgetattr(fd, &options);

	cfsetispeed(&options, BAUD_RATE);
	cfsetospeed(&options, BAUD_RATE);

	options.c_cflag |= (CLOCAL | CREAD);

	tcsetattr(fd, TCSANOW, &options);	
} 

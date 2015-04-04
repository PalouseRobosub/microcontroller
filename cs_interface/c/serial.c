#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>


//local includes
#include "serial.h"

//private functions
void s_configure(int fd); 


int s_open(char *port)
{
	int fd;

	fd = open(port, O_RDWR | O_NOCTTY | O_NDELAY);

	if(fd < 0)
	{
		return fd;
	}

	s_configure(fd);
	
	return fd;
}

void s_configure(int fd) 
{
	struct termios options;

	tcgetattr(fd, &options);

	cfsetispeed(&options, BAUD_RATE);
	cfsetospeed(&options, BAUD_RATE);

	options.c_cflag |= (CLOCAL | CREAD);

	tcsetattr(fd, TCSANOW, &options);	
}


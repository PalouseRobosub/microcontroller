
#include <cstring>
#include <cstdlib>
#include <cstdio>
extern "C" {
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
}
//local includes
#include "serial.h"

/*******************************
 * Serial Class
 *******************************/
Serial::Serial(char *port_name)
{
	this->port_fd = open(port_name, O_RDWR | O_NOCTTY | O_NDELAY);
	if (this->port_fd < 0)
	{
		fprintf(stderr, "could not open port: \'%s\'\n", port_name);
		exit(1);
	}

	this->configure();	
}

Serial::~Serial()
{
	close(this->port_fd);
}


int Serial::swrite(void *buf, int size)
{
	return write(this->port_fd, buf, size);	
}

int Serial::sread(void *buf, int size)
{
	return read(this->port_fd, buf, size);
}

void Serial::configure()
{
	struct termios options;
	int fd = this->port_fd;

	printf("configuring port...");

	tcgetattr(fd, &options);

	cfsetispeed(&options, BAUD_RATE);
	cfsetospeed(&options, BAUD_RATE);

	options.c_cflag |= (CLOCAL | CREAD);

	tcsetattr(fd, TCSANOW, &options);	
	fcntl(fd, F_SETFL, 0);

	printf("done!\n");
}


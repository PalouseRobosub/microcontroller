
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


int Serial::swrite(char *buf, int num)
{
	int i;

	i = write(this->port_fd, buf, num);
	if (i != num)
		fprintf(stderr, "serial write error, tried to write %d bytes, actually wrote %d\n", num, i);
	return i;	
}

int Serial::sread(char *buf, int num)
{
	int got, i;
	got = 0;
	printf("getting %d bytes\n", num);
	while (got < num)
	{
		i = read(this->port_fd, buf+got, num - got);
		printf("%d read,", i);
		got += i;
		printf("%d of %d bytes\n", got, num);
	}

	return got;
}

void Serial::configure()
{
	struct termios options;
	int fd = this->port_fd;

	printf("configuring port...");
	
	memset(&options, sizeof(options), 0); //clear out old configuration

	cfsetispeed(&options, BAUD_RATE);
	cfsetospeed(&options, BAUD_RATE);

	cfmakeraw(&options);
	
	tcsetattr(fd, TCSANOW, &options);	
	fcntl(fd, F_SETFL, 0);

	printf("done!\n");
}


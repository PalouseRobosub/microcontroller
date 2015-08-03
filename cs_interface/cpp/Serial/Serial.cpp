#include "Serial.hpp"
#include <iostream>

    Serial::Serial(const char *port_name, int baud_rate)
    {
        this->port_fd = open(port_name, O_RDWR | O_NOCTTY | O_NDELAY);
        if (this->port_fd < 0)
		{
			fprintf(stderr, "could not open port: \'%s\'\n", port_name);
			exit(1);
		}     

        this->configure(baud_rate);
    }

    Serial::~Serial()
    {
        close(this->port_fd);
    }


    int Serial::swrite(uint8_t *buf, int num)
    {
		int i;

        i = write(this->port_fd, buf, num);
		if (i != num)
			fprintf(stderr, "serial write error, tried to write %d bytes, actually wrote %d\n", num, i);


		return i;
    }

    int Serial::sread(uint8_t *buf, int num)
    {
		int got, i;
		got = 0;

		while(got < num)
		{
        	i = read(this->port_fd, buf+got, num-got);
			got += i;
		}

		return got;
    }

    void Serial::configure(int baud_rate)
    {
        struct termios options;
        int fd = this->port_fd;

        printf("configuring port");

		memset(&options, sizeof(options), 0);

        cfsetispeed(&options, baud_rate);
        cfsetospeed(&options, baud_rate);

		cfmakeraw(&options);

        tcsetattr(fd, TCSANOW, &options);
        fcntl(fd, F_SETFL, 0);

        printf("done!\n");
    }

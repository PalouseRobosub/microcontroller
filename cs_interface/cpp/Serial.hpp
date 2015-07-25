#ifndef __SERIAL_H__
#define __SERIAL_H__

#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <stdint.h>

    class Serial
    {
        public:
            Serial(const char *port_name, int baud_rate=B115200);
            ~Serial();
            int swrite(uint8_t *buf, int num);
            int sread(uint8_t *buf, int num);

        private:
            //private methods
            void configure(int baud_rate);

            //private members
            int port_fd;
            char *port_name;
    };

#endif

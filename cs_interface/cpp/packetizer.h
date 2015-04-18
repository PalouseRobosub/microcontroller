#ifndef __PACKETIZER_H__
#define __PACKETIZER_H__

#include "serial.h"

class Packetizer
{
	public:
		Packetizer(char *port_name, char start_byte = 0x0A);
		~Packetizer(void);
		int get(char *buf, char *num);
		int send(char *buf, char num);

	private:
		void get_lock(void);
		
		Serial serial;
		char start_byte;
};

#endif 

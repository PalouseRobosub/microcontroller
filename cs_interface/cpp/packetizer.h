#ifndef __PACKETIZER_H__
#define __PACKETIZER_H__

#include "serial.h"

//defines
#define MAX_PACKET_SIZE 255

class Packetizer
{
	public:
		Packetizer(char *port_name, char control_byte = 0x0A);
		~Packetizer(void);
		int get(char buf[MAX_PACKET_SIZE], char *num);
		int send(char buf[MAX_PACKET_SIZE], char num);

	private:
		void get_lock(void);
		
		Serial serial;
		char control_byte;
};

#endif 

#include <iostream>

#include "../../LED/led.h"
#include "packetizer.h"

#define WAIT 1000

typedef unsigned char uint8;

int main(int argc, char *argv[])
{
	uint8 temp[1] = {set};
	uint8 blue[4] = {1, 0x80, 0x80, 0xFF};
	uint8 red[4] = {1, 0x80, 0xFF, 0x80};
	uint8 green[4] = {1, 0xFF, 0x80, 0x80};
	Packetizer outstream((char*) "/dev/ttyUSB0", '\n');
	while(true)
	{
		outstream.send((char*) red, 4);
		for(int i = 0; i < WAIT; i++);
		outstream.send((char*) blue, 4);
		for(int i = 0; i < WAIT; i++);
		outstream.send((char*) green, 4);
		for(int i = 0; i < WAIT; i++);
	}
	/*while(true) {
	std::cout << "Enter Color in format green, red, blue" << std::endl;
	std::cin >> green >> red >> blue;
	led_buf[1] = (uint8) green | 0x80;
	led_buf[2] = (uint8) red | 0x80;
	led_buf[3] = (uint8) blue | 0x80;

	outstream.send((char*) led_buf, 4);
	outstream.send((char*) temp, 1);
	std::cin.clear();
	}*/

	return 0;
}

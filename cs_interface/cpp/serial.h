#ifndef __SERIAL_H__
#define __SERIAL_H__

typedef char u8;

//Defines
#ifndef BAUD_RATE
#define BAUD_RATE B115200
//#define BAUD_RATE B19200
//#define BAUD_RATE B9600
#endif


class Serial
{
	public:
		Serial(char *port_name);
		~Serial(void);
		int swrite(void *buf, int size);
		int sread(void *buf, int size);

	private:
		//private methods
		void configure(void);	
		
		

		//private members
		int port_fd;
		char *port_name;
};

#endif 

#include <string.h>
#include <stdio.h>

#include "packetizer.h"

typedef enum
{
	ACCEL,
	GYRO,
	MAGNETO,
	TEMP,
	PRESSURE
} SENSOR;

typedef struct Data
{
	int x, y, z;
} DATA;

int main(int argc, char**argv)
{
	int size;
	char port[64] = "/dev/ttyUSB0";
	char msg[256];
	DATA accel, gyro, magneto;

	if(argc >= 2)
	{
		strcpy(port, argv[1]);
	}
	Packetizer p(port);

	printf("serial port opened: %s\n", port);


	
	while (1)
	{
		size = p.get(msg);
		printf("received: %s\n", msg);
		switch(msg[1])
		{
			case ACCEL:
				if(size != 7)
					break;
				accel.x = msg[2]<<8 | msg[1];
				accel.y = msg[4]<<8 | msg[3];
				accel.z = msg[6]<<8 | msg[5];
				break;

			case GYRO:
				gyro.x = msg[1]<<8 | msg[2];
				gyro.y = msg[3]<<8 | msg[4];
				gyro.z = msg[5]<<8 | msg[6];
				break;
			case MAGNETO:
				magneto.x = msg[1]<<8 | msg[2]; //Y value comes after Z value
				magneto.y = msg[5]<<8 | msg[6];
				magneto.z = msg[3]<<8 | msg[4];
				break;
			case TEMP:
				break;
			case PRESSURE:
				break;
			default:
				perror("Invalid Sensor Code");


		}
	}

	printf("done receiving!\n");

	return 0;
}


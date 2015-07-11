#include "sensor.h"

typedef enum
{
	ACCEL,
	GYRO,
	MAGNETO,
	TEMP,
	PRESSURE,
	DEPTH
} SENSOR;

typedef struct Data
{
	int x, y, z;
} DATA;

int main(int argc, char**argv)
{
	int size;
	char port[64] = "/dev/ttyUSB2";
	char msg[256];
	float ypr[3];
	Gyro_Accel_Test gat = Gyro_Accel_Test();
	DATA magneto;
	short int depth;

	if(argc >= 2)
	{
		strcpy(port, argv[1]);
	}
	Packetizer p(port);

	printf("serial port opened: %s\n", port);



	while (1)
	{
		size = p.get(msg);
		switch(msg[0])
		{
			case ACCEL:
				if(size != 7)
					break;
				gat.accel.updateAccel(msg[2]<<8 | msg[1], msg[4]<<8 | msg[3], msg[6]<<8 | msg[5]);
				// accel.x = msg[2]<<8 | msg[1];
				// accel.y = msg[4]<<8 | msg[3];
				// accel.z = msg[6]<<8 | msg[5];
				break;

			case GYRO:
				if(size != 7)
					break;
				gat.gyro.updateGyro(msg[1]<<8 | msg[2], msg[3]<<8 | msg[4], msg[5]<<8 | msg[6]);
				// gyro.x = msg[1]<<8 | msg[2];
				// gyro.y = msg[3]<<8 | msg[4];
				// gyro.z = msg[5]<<8 | msg[6];
				break;
			case MAGNETO:
				if(size != 7)
					break;
				//For now update the quaternion values when MAGNETO sends a packets
				gat.getYPR(ypr);
				std::cout << "YAW: " << ypr[0] << std::char(167)
						  << " PITCH: " << ypr[1] << std::char(167)
						  << " ROLL: " << ypr[2] << std::char(167)
						  << std::endl;
				magneto.x = msg[1]<<8 | msg[2]; //Y value comes after Z value
				magneto.y = msg[5]<<8 | msg[6];
				magneto.z = msg[3]<<8 | msg[4];
				break;
			case TEMP:
				break;
			case PRESSURE:
				break;
			case DEPTH:
				depth = msg[1]<<8 | msg[2];
				printf("depth: %d\n", depth);
				break;
			default:
				perror("Invalid Sensor Code");


		}
	}


	return 0;
}

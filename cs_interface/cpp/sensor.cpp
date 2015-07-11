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

	float tmpOffset[] = {0,0,0};

	for (int i = 0; i < 300; ++i)
	{
		p.get(msg);
		while(msg[0] != GYRO){p.get(msg);} //Wait for next gyro packet
		tmpOffset[0] += msg[2]<<8 | msg[1];
		tmpOffset[1] += msg[4]<<8 | msg[3];
		tmpOffset[2] += msg[6]<<8 | msg[5];
	}

	gat.gyro.offset[0] = -tmpOffset[0] / 10;
	gat.gyro.offset[1] = -tmpOffset[1] / 10;
	gat.gyro.offset[2] = -tmpOffset[2] / 10;

	while (1)
	{
		size = p.get(msg);
		switch(msg[0])
		{
			case ACCEL:
				//printf("hello\n");
				//std::cout << "fuck you says accel" << size << std::endl;
				if(size != 7)
					break;
				gat.accel.updateAccel(msg[2]<<8 | msg[1], msg[4]<<8 | msg[3], msg[6]<<8 | msg[5]);
				// accel.x = msg[2]<<8 | msg[1];
				// accel.y = msg[4]<<8 | msg[3];
				// accel.z = msg[6]<<8 | msg[5];
				break;

			case GYRO:
				//std::cout << "fuck you says gyro" << size << std::endl;
				if(size != 7)
					break;
				gat.gyro.updateGyro(msg[2]<<8 | msg[1], msg[4]<<8 | msg[3], msg[6]<<8 | msg[5]);
				// gyro.x = msg[1]<<8 | msg[2];
				// gyro.y = msg[3]<<8 | msg[4];
				// gyro.z = msg[5]<<8 | msg[6];
				break;
			case MAGNETO:
				//std::cout << "fuck you says magneto" << size << std::endl;
				if(size != 7)
					break;
				//For now update the quaternion values when MAGNETO sends a packets
				gat.getEuler(ypr);
				std::cout << "Psi: " << ypr[0] 
						  << " Theta: " << ypr[1] 
						  << " Phi: " << ypr[2]
						  << std::endl;
				gat.getYPR(ypr);
				std::cout << "YAW: " << ypr[0]
					  << " PITCH: " << ypr[1]
					  << " ROLL: " << ypr[2]
					  << std::endl;
				gat.getAngles(ypr);
				std::cout << "X: " << ypr[0]
					  << " Y: " << ypr[1]
					  << " Z: " << ypr[2]
					  << std::endl;
				magneto.x = msg[2]<<8 | msg[1]; //Y value comes after Z value
				magneto.y = msg[4]<<8 | msg[3];
				magneto.z = msg[6]<<8 | msg[5];
				break;
			case TEMP:
				break;
			case PRESSURE:
				break;
			case DEPTH:
				depth = msg[2]<<8 | msg[1];
				break;
			default:
				perror("Invalid Sensor Code");


		}
	}


	return 0;
}

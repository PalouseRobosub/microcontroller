#include "sensor.h"

using std::cout;
using std::endl;
using std::cerr;

typedef enum
{
	ACCEL,
	GYRO,
	MAGNETO,
	TEMP,
	PRESSURE,
	DEPTH
} SENSOR;

//typedef struct Data
//{
//	int x, y, z;
//} DATA;

int main(int argc, char**argv)
{
	int size;
	char port[64] = "/dev/ttyUSB2";
	char msg[256];
	float ypr[4];
	Gyro_Accel_Test gat = Gyro_Accel_Test();
	short int depth;

	if(argc >= 2)
	{
		strcpy(port, argv[1]);
	}
	Packetizer p(port);

	printf("serial port opened: %s\n", port);

	int16_t tmpOffset[] = {0,0,0};
	cout << "Getting Samples for Gyro Offset" << endl;
	for (int i = 0; i < 100; ++i)
	{
		p.get(msg);
		while(msg[0] != GYRO){p.get(msg);} //Wait for next gyro packet
		tmpOffset[0] += msg[2]<<8 | msg[1];
		tmpOffset[1] += msg[4]<<8 | msg[3];
		tmpOffset[2] += msg[6]<<8 | msg[5];
		if(i % 10 == 0) cout << tmpOffset[0] << " " << tmpOffset[1] << " " << tmpOffset[2] << endl;
	}

	gat.gyro.offset[0] = -tmpOffset[0] / 10;
	gat.gyro.offset[1] = -tmpOffset[1] / 10;
	gat.gyro.offset[2] = -tmpOffset[2] / 10;

    //Loop to get all of the messages
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
//				cout << "RAW ACCEL: " << gat.accel.x_raw << " " << gat.accel.y_raw << " " << gat.accel.z_raw << endl;
				// accel.x = msg[2]<<8 | msg[1];
				// accel.y = msg[4]<<8 | msg[3];
				// accel.z = msg[6]<<8 | msg[5];
				break;

			case GYRO:
				//std::cout << "fuck you says gyro" << size << std::endl;
				if(size != 7)
					break;
				gat.gyro.updateGyro(msg[2]<<8 | msg[1], msg[4]<<8 | msg[3], msg[6]<<8 | msg[5]);
//				cout << "RAW GYRO: " << gat.gyro.raw_x << " " << gat.gyro.raw_y << " " << gat.gyro.raw_z << endl;
				// gyro.x = msg[1]<<8 | msg[2];
				// gyro.y = msg[3]<<8 | msg[4];
				// gyro.z = msg[5]<<8 | msg[6];
				break;
			case MAGNETO:
				//std::cout << "fuck you says magneto" << size << std::endl;
				if(size != 7)
					break;

				//  magneto.x = msg[2]<<8 | msg[1]; //Y value comes after Z value
				//  magneto.y = msg[4]<<8 | msg[3];
				//  magneto.z = msg[6]<<8 | msg[5];
				gat.mag.updateMag(msg[2]<<8 | msg[1], msg[4]<<8 | msg[3], msg[6]<<8 | msg[5]);
//				cout << "RAW MAG: " << gat.mag.x_raw << " " << gat.mag.y_raw << " " << gat.mag.z_raw << endl;
				//For now update the quaternion values when MAGNETO sends a packet
				gat.update();

				gat.getEuler(ypr);
				cout << "Psi: " << ypr[0]
					 << " Theta: " << ypr[1]
					 << " Phi: " << ypr[2]
					 << endl;
				gat.getYPR(ypr);
				cout << "YAW: " << ypr[0]
					 << " PITCH: " << ypr[1]
					 << " ROLL: " << ypr[2]
					 << endl;
				gat.getQVals(ypr);
				cout << "Q0: " << ypr[0]
					 << " Q1: " << ypr[1]
					 << " Q2: " << ypr[2]
					 << " Q3: " << ypr[3]
					 << endl;
				break;
			case TEMP:
				break;
			case PRESSURE:
				break;
			case DEPTH:
				depth = msg[2]<<8 | msg[1];
				cout << "DEPTH: " << depth << endl << endl;
				break;
			default:
				cerr << "Invalid Sensor Code" << endl;


		}
	}


	return 0;
}

#include "sensor.h"

using std::cout;
using std::endl;
using std::cerr;

enum
{
	ACCEL,
	GYRO,
	MAGNETO,
	TEMP,
	PRESSURE,
	DEPTH
};

//typedef struct Data
//{
//	int x, y, z;
//} DATA;

int main(int argc, char**argv)
{
	int size;
	char port[64] = "/dev/ttyUSB4";
	char msg[256];
	float ypr[4];
	Gyro_Accel_Test gat = Gyro_Accel_Test();
	short int depth;
	unsigned char *data = (unsigned char*) msg;	

	if(argc >= 2)
	{
		strcpy(port, argv[1]);
	}
	Packetizer p(port);

	printf("serial port opened: %s\n", port);

	int tmpOffset[] = {0,0,0};
	cout << "Getting Samples for Gyro Offset" << endl;
	for (int i = 0; i < 50; ++i)
	{
		p.get(msg);
		while(msg[0] != GYRO){p.get(msg);} //Wait for next gyro packet
		tmpOffset[0] += (short int) (data[2]<<8 | data[1]);
		tmpOffset[1] += (short int) (data[4]<<8 | data[3]);
		tmpOffset[2] += (short int) (data[6]<<8 | data[5]);
//		if(i % 10 == 0) cout << tmpOffset[0] << " " << tmpOffset[1] << " " << tmpOffset[2] << endl;
	}

	gat.gyro.offset[0] = (int16_t) (-tmpOffset[0] / 50);
	gat.gyro.offset[1] = (int16_t) (-tmpOffset[1] / 50);
	gat.gyro.offset[2] = (int16_t) (-tmpOffset[2] / 50);

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
				gat.accel.updateAccel(data[2]<<8 | data[1],  data[4]<<8 | data[3], data[6]<<8 | data[5]);
				cout << "A: " << gat.accel.x << " " << gat.accel.y << " " << gat.accel.z << endl;
				// accel.x = msg[2]<<8 | msg[1];
				// accel.y = msg[4]<<8 | msg[3];
				// accel.z = msg[6]<<8 | msg[5];
				break;

			case GYRO:
				//std::cout << "fuck you says gyro" << size << std::endl;
				if(size != 7)
					break;
				gat.gyro.updateGyro(data[2]<<8 | data[1], data[4]<<8 | data[3], data[6]<<8 | data[5]);
				cout << "G: " << gat.gyro.fx << " " << gat.gyro.fy << " " << gat.gyro.fz << endl;
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
				gat.mag.updateMag(data[2]<<8 | data[1], data[4]<<8 | data[3], data[6]<<8 | data[5]);
				cout << "M: " << gat.mag.x << " " << gat.mag.y << " " << gat.mag.z << endl;
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
				depth = data[2]<<8 | data[1];
				cout << "DEPTH: " << depth << endl << endl;
				break;
			default:
				cerr << "Invalid Sensor Code" << endl;


		}
	}


	return 0;
}

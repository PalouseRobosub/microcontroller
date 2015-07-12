#ifndef __GYRO_H__
#define __GYRO_H__

#include <thread> //for std::this_thread::sleep_for
#include <chrono> //for std::chrono::milliseconds

class Gyroscope
{
public:
	float gain[3];
	float x, y, z;
	int raw_x, raw_y, raw_z;
	int offset[3];
	float polarity[3];

	Gyroscope();

	void zeroCalibrate(unsigned int totSamples, unsigned int sampleDelayMS);
	void updateGyro(int gyroX, int gyroY, int gyroZ);
	void setRevPolarity(bool Xpol, bool Ypol, bool Zpol);
};
#endif

#ifndef __GYRO_H__
#define __GYRO_H__

#include <thread> //for std::this_thread::sleep_for
#include <chrono> //for std::chrono::milliseconds
#include <cstdint>

using std::int16_t;

class Gyroscope
{
public:
	float gain[3];
	float x, y, z;
	int16_t raw_x, raw_y, raw_z;
	int16_t offset[3];
	float polarity[3];

	Gyroscope();

	void zeroCalibrate(unsigned int totSamples, unsigned int sampleDelayMS);
	void updateGyro(int16_t gyroX, int16_t gyroY, int16_t gyroZ);
	void setRevPolarity(bool Xpol, bool Ypol, bool Zpol);
};
#endif

#include "gyroscope.h"

Gyroscope::Gyroscope()
{
	gain[0] = 1.0;
	gain[1] = 1.0;
	gain[2] = 1.0;

	polarity[0] = 1.0;
	polarity[1] = 1.0;
	polarity[2] = 1.0;

	offset[0] = 0;
	offset[1] = 0;
	offset[2] = 0;
}

//This function is used to set the offsets for the gyro
//It is based on readings at a set interval
//It should only be called when the IMU is not moving
//TODO: Move outside the gyro class so that it has access to the raw packets instead
//		of using the internal raw values
void Gyroscope::zeroCalibrate(unsigned int totSamples, unsigned int sampleDelayMS)
{
	int xyz[3];
	float tmpOffset[] = {0, 0, 0};

	for (int i = 0; i < totSamples; ++i)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(sampleDelayMS));
		tmpOffset[0] += raw_x;
		tmpOffset[1] += raw_y;
		tmpOffset[2] += raw_z;
	}
	offset[0] = -tmpOffset[0] / totSamples;
	offset[1] = -tmpOffset[1] / totSamples;
	offset[2] = -tmpOffset[2] / totSamples;
}

void Gyroscope::updateGyro(float gyroX, float gyroY, float gyroZ)
{
	raw_x = gyroX;
	x = gyroX;
	raw_y = gyroY;
	y = gyroY;
	raw_z = gyroZ;
	z = gyroZ;

	x += offset[0];
	y += offset[1];
	z += offset[2];

	x = x / 14.375 * polarity[0] * gain[0];
	y = y / 14.375 * polarity[1] * gain[1];
	z = z / 14.375 * polarity[2] * gain[2];
}


void Gyroscope::setRevPolarity(bool Xpol, bool Ypol, bool Zpol)
{
	polarity[0] = Xpol ? -1 : 1;
	polarity[1] = Ypol ? -1 : 1;
	polarity[2] = Zpol ? -1 : 1;
}

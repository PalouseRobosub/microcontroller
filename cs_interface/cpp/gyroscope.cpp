#include "gyro.h"

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


void Gyroscope::zeroCalibrate(unsigned int totSamples, unsigned int sampleDelayMS)
{
	int xyz[3];
	float tmpOffset[] = {0, 0, 0};

	for (int i = 0; i < totSamples; ++i)
	{
		delay(sampleDelayMS);
		readGyroRaw(xyz);
		tmpOffset[0] += xyz[0];
		tmpOffset[1] += xyz[1];
		tmpOffset[2] += xyz[2];
	}
	offset[0] = -tmpOffsets[0] / totSamples;
	offset[1] = -tmpOffsets[1] / totSamples;
	offset[2] = -tmpOffsets[2] / totSamples;
}

void Gyroscope::updateGyro(int *gyroX, int *gyroY, int *gyroZ)
{
	readGyroRaw(gyroX, gyroY, gyroZ);
	*gyroX += offset[0];
	*gyroY += offset[1];
	*gyroZ += offset[2];

	*gyroX = *gyroX / 14.375 * polarity[0] * gain[0];
	*gyroY = *gyroY / 14.375 * polarity[1] * gain[1];
	*gyroZ = *gyroZ / 14.375 * polarity[2] * gain[2];
}


void setRevPolarity(bool Xpol, bool Ypol, bool Zpol)
{
  polarities[0] = _Xpol ? -1 : 1;
  polarities[1] = _Ypol ? -1 : 1;
  polarities[2] = _Zpol ? -1 : 1;
}

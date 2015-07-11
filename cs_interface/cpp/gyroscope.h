#ifndef __GYRO_H__
#define __GYRO_H__

class Gyroscope {

pubic:
	float gain[3];
	float x, y, z;
	int offset[3];
	float polarity[3];

	Gyroscope();


	void zeroCalibrate(unsigned int totSamples, unsigned int sampleDelayMS);
	void updateGyro(int *gyroX, int *gyroY, int *gyroZ);
	void setRevPolarity(bool Xpol, bool Ypol, bool Zpol);
	

};
#endif __GYRO_H__

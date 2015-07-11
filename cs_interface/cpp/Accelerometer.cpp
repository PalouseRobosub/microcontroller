#include "Accelerometer.h"

Accelerometer::Accelerometer()
{
    gain[0] = 0.00376390;
    gain[1] = 0.00376009;
    gain[2] = 0.00349265;
}

void Accelerometer::updateAccel(float ax, float ay, float az)
{
    x = gain[0] * (ax);
    y = gain[1] * (ay);
    z = gain[2] * (az);
}

void Accelerometer::getXYZ(float *ax, float *ay, float *az)
{
    *ax = x;
    *ay = y;
    *az = z;
}

#include "Accelerometer.h"

Accelerometer::Accelerometer()
{
    alpha = 0.5;
    gain[0] = 0.00376390;
    gain[1] = 0.00376009;
    gain[2] = 0.00349265;
}

void Accelerometer::updateAccel(int ax, int ay, int az)
{
    x_raw = ax;
    y_raw = ay;
    z_raw = az;

    //Low pass filtering
    fx = -ax*alpha + fx*(1.0-alpha);
    fy = ay*alpha + fy*(1.0-alpha);
    fz = -az*alpha + fz*(1.0-alpha);

    x = gain[0] * (-fx);
    y = gain[1] * (fy);
    z = gain[2] * (-fz);
}

void Accelerometer::getXYZ(float *ax, float *ay, float *az)
{
    *ax = x;
    *ay = y;
    *az = z;
}

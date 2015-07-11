#ifndef ACCEL_H
#define ACCEL_H

class Accelerometer
{
public:
    Accelerometer();
    float gain[3];
    float x,y,z;
    void updateAccel(float ax, float ay, float az);
    void getXYZ(float *ax, float *ay, float *az);
};

#endif

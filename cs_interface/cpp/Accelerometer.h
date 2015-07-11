#ifndef ACCEL_H
#define ACCEL_H

class Accelerometer
{
public:
    Accelerometer();
    float gain[3];
    float x,y,z;
    void updateAccel(int *ax, int *ay, int *az);
    void getXYZ(float *ax, float *ay, float *az);
};

#endif

#ifndef ACCEL_H
#define ACCEL_H

class Accelerometer
{
public:
    Accelerometer();
    float alpha;
    float gain[3];
    float x,y,z;
    int x_raw, y_raw, z_raw;
    int fx, fy, fz; //Filter variables
    void updateAccel(int ax, int ay, int az);
    void getXYZ(float *ax, float *ay, float *az);
};

#endif

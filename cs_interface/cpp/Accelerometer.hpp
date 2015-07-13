#ifndef ACCEL_H
#define ACCEL_H

#include <cstdint>

using std::int16_t;

class Accelerometer
{
public:
    Accelerometer();
    float gain[3];
    float x,y,z;
    int16_t x_raw, y_raw, z_raw;
    void updateAccel(int16_t ax, int16_t ay, int16_t az);
    void getXYZ(float *ax, float *ay, float *az);
};

#endif

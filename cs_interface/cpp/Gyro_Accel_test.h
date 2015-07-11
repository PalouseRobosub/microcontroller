#ifndef
#define
#include "Accelerometer.h"
#include "gyro.h"
#include <cmath>

#define twoKpDef (2.0f * 0.5f) //2 * proportional gain
#define twoKiDef (2.0f * 0.1f) //2 * integral gain

#define PI 3.14159265358979323846264 //PI definition

class Gyro_Accel_Test
{
public:
    Gyro_Accel_Test();
    void getQ(float * q);
    void getEuler(float * angles);
    void getYPR(float *ypr);
    void getAngles(float * angles);
    float invSqrt(float number);

    Accelerometer accel;
    Gyroscope gyro;

private:
    void updateQuaternion(float gx, float gy, float gz, float ax, float ay, float az);
    volatile float q0, q1, q2, q3; //Quaternion
    volatile float twoKp;          //2 * proportional gain (Kp)
    volatile float twoKi;          //2 * integral gain (Ki)
    volatile float integralFBx, integralFBy, integralFBz;
};

#endif

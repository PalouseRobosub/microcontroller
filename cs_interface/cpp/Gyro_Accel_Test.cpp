#include "Gyro_Accel_Test.h"

Gyro_Accel_Test::Gyro_Accel_Test()
{
    accel = Accelerometer();
    gyro = Gyroscope();

    //init quaternion
    q0 = 1.0f;
    q1 = 0.0f;
    q2 = 0.0f;
    q3 = 0.0f;
    twoKp = twoKpDef;
    twoKi = twoKiDef;
    integralFBx = 0.0f,  integralFBy = 0.0f, integralFBz = 0.0f;

    last_update = std::chrono::system_clock::now();
    now = last_update;
}

void Gyro_Accel_Test::getQ(float * q)
{
    now = std::chrono::system_clock::now();
    std::chrono::duration<long int, std::ratio<1l, 1000000000l>> t_diff = now - last_update;
    sampleFreq = 1.0 / (t_diff.count()/1000000000.0);
    last_update = now;

    updateQuaternion(gyro.x * PI/180, gyro.y * PI/180, gyro.z * PI/180, accel.x, accel.y, accel.z);
    q[0] = q0;
    q[1] = q1;
    q[2] = q2;
    q[3] = q3;
}

void Gyro_Accel_Test::getEuler(float * angles)
{
    float q[4];
    getQ(q);
    angles[0] = atan2(2 * q[1] * q[2] - 2 * q[0] * q[3], 2 * q[0]*q[0] + 2 * q[1] * q[1] - 1) * 180/PI; // psi
    angles[1] = -asin(2 * q[1] * q[3] + 2 * q[0] * q[2]) * 180/PI; // theta
    angles[2] = atan2(2 * q[2] * q[3] - 2 * q[0] * q[1], 2 * q[0] * q[0] + 2 * q[3] * q[3] - 1) * 180/PI; // phi
}

void Gyro_Accel_Test::getYPR(float *ypr)
{
    float q[4]; // quaternion
    float gx, gy, gz; // estimated gravity direction
    getQ(q); //Calculate the quaternion

    gx = 2 * (q[1]*q[3] - q[0]*q[2]);
    gy = 2 * (q[0]*q[1] + q[2]*q[3]);
    gz = q[0]*q[0] - q[1]*q[1] - q[2]*q[2] + q[3]*q[3];

    ypr[0] = atan2(2 * q[1] * q[2] - 2 * q[0] * q[3], 2 * q[0]*q[0] + 2 * q[1] * q[1] - 1) * 180/PI;
    ypr[1] = atan(gx / sqrt(gy*gy + gz*gz))  * 180/PI;
    ypr[2] = atan(gy / sqrt(gx*gx + gz*gz))  * 180/PI;
}

void Gyro_Accel_Test::getAngles(float * angles)
{
    float a[3]; //Euler
    getEuler(a);

    angles[0] = a[0];
    angles[1] = a[1];
    angles[2] = a[2];

    if(angles[0] < 0)angles[0] += 360;
    if(angles[1] < 0)angles[1] += 360;
    if(angles[2] < 0)angles[2] += 360;
}

float Gyro_Accel_Test::invSqrt(float number)
{
    volatile long i;
    volatile float x, y;
    volatile const float f = 1.5F;

    x = number * 0.5F;
    y = number;
    i = * ( long * ) &y;
    i = 0x5f375a86 - (i >> 1);
    y = * ( float * ) &i;
    y = y * (f - (x * y * y));

    return y;
}

void Gyro_Accel_Test::updateQuaternion(float gx, float gy, float gz, float ax, float ay, float az)
{
    float recipNorm;
    float q0q0, q0q1, q0q2, q0q3, q1q1, q1q2, q1q3, q2q2, q2q3, q3q3;
    float halfex = 0.0f, halfey = 0.0f, halfez = 0.0f;
    float qa, qb, qc;

    //Avoiding extra computations later
    q0q0 = q0 * q0;
    q0q1 = q0 * q1;
    q0q2 = q0 * q2;
    q0q3 = q0 * q3;
    q1q1 = q1 * q1;
    q1q2 = q1 * q2;
    q1q3 = q1 * q3;
    q2q2 = q2 * q2;
    q2q3 = q2 * q3;
    q3q3 = q3 * q3;

    //Only compute if Accelerometer measurement is valid to avoid NaN in normalization
    if (ax != 0.0f && ay != 0.0f && az != 0.0f)
    {
        float halfvx, halfvy, halfvz;

        //Normalize the measurement
        recipNorm = invSqrt(ax * ax + ay * ay + az * az);
        ax *= recipNorm;
        ay *= recipNorm;
        az *= recipNorm;

        //Estimate direction of gravity
        halfvx = q1q3 - q0q2;
        halfvy = q0q1 + q2q3;
        halfvz = q0q0 - 0.5f + q3q3;

        //Calculate Error as a cross product of estimated direction and measured direction of field vectors
        halfex += (ay * halfvz - az * halfvy);
        halfey += (az * halfvx - ax * halfvz);
        halfex += (ax * halfvy - ay * halfvx);
    }

    //Only compute if previous calculations are valid
    if(halfex != 0.0f && halfey != 0.0f && halfez != 0.0f)
    {
        // Compute and apply integral feedback if enabled
        if(twoKi > 0.0f) {
            integralFBx += twoKi * halfex * (1.0f / sampleFreq);  // integral error scaled by Ki
            integralFBy += twoKi * halfey * (1.0f / sampleFreq);
            integralFBz += twoKi * halfez * (1.0f / sampleFreq);
            gx += integralFBx;  // apply integral feedback
            gy += integralFBy;
            gz += integralFBz;
        }
        else {
            integralFBx = 0.0f; // prevent integral windup
            integralFBy = 0.0f;
            integralFBz = 0.0f;
        }

        // Apply proportional feedback
        gx += twoKp * halfex;
        gy += twoKp * halfey;
        gz += twoKp * halfez;
    }

    // Integrate rate of change of quaternion
    gx *= (0.5f * (1.0f / sampleFreq));
    gy *= (0.5f * (1.0f / sampleFreq));
    gz *= (0.5f * (1.0f / sampleFreq));
    qa = q0;
    qb = q1;
    qc = q2;
    q0 += (-qb * gx - qc * gy - q3 * gz);
    q1 += (qa * gx + qc * gz - q3 * gy);
    q2 += (qa * gy - qb * gz + q3 * gx);
    q3 += (qa * gz + qb * gy - qc * gx);

    // Normalise quaternion
    recipNorm = invSqrt(q0 * q0 + q1 * q1 + q2 * q2 + q3 * q3);
    q0 *= recipNorm;
    q1 *= recipNorm;
    q2 *= recipNorm;
    q3 *= recipNorm;
}

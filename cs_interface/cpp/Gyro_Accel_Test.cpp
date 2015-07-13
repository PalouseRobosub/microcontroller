#include "Gyro_Accel_Test.h"
#include <iostream>

Gyro_Accel_Test::Gyro_Accel_Test()
{
    accel = Accelerometer();
    gyro = Gyroscope();
    mag = Magnetometer();
    mag.SetScale(0.88);

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

void Gyro_Accel_Test::update()
{
    float q[4];
    getQ(q);

    //Update Euler angles
    psi = atan2(2 * q1 * q2 - 2 * q0 * q3, 2 * q0*q0 + 2 * q1 * q1 - 1) * 180/PI;
    theta = -asin(2 * q1 * q3 + 2 * q0 * q2) * 180/PI;
    phi = atan2(2 * q2 * q3 - 2 * q0 * q1, 2 * q0 * q0 + 2 * q3 * q3 - 1) * 180/PI;

    //Update Yaw Pitch roll
    //Estimate gravity
    float gx, gy, gz; // estimated gravity direction

    gx = 2 * (q1*q3 - q0*q2);
    gy = 2 * (q0*q1 + q2*q3);
    gz = q0*q0 - q1*q1 - q2*q2 + q3*q3;

    yaw = atan2(2 * q1 * q2 - 2 * q0 * q3, 2 * q0*q0 + 2 * q1 * q1 - 1) * 180/PI;
    pitch = atan(gx / sqrt(gy*gy + gz*gz))  * 180/PI;
    roll = atan(gy / sqrt(gx*gx + gz*gz))  * 180/PI;

}

void Gyro_Accel_Test::getQ(float * q)
{
    now = std::chrono::system_clock::now();
    std::chrono::duration<long int, std::ratio<1l, 1000000000l>> t_diff = now - last_update;
    sampleFreq = 1.0 / (t_diff.count()/1000000000.0);
    last_update = now;

    updateQuaternion(gyro.x * PI/180, gyro.y * PI/180, gyro.z * PI/180, accel.x, accel.y, accel.z, mag.x, mag.y, mag.z);
    q[0] = q0;
    q[1] = q1;
    q[2] = q2;
    q[3] = q3;
}

void Gyro_Accel_Test::getEuler(float * angles)
{
    angles[0] = psi;
    angles[1] = theta;
    angles[2] = phi;
}

void Gyro_Accel_Test::getYPR(float *ypr)
{
    ypr[0] = yaw;
    ypr[1] = pitch;
    ypr[2] = roll;
}

void Gyro_Accel_Test::getQVals(float * qs)
{
    qs[0] = q0;
    qs[1] = q1;
    qs[2] = q2;
    qs[3] = q3;
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

void Gyro_Accel_Test::updateQuaternion(float gx, float gy, float gz, float ax, float ay, float az, float mx, float my, float mz)
{
    float recipNorm;
    float q0q0, q0q1, q0q2, q0q3, q1q1, q1q2, q1q3, q2q2, q2q3, q3q3;
    float halfex = 0.0f, halfey = 0.0f, halfez = 0.0f;
    float qa, qb, qc;

    // Auxiliary variables to avoid repeated arithmetic
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

    // Use magnetometer measurement only when valid (avoids NaN in magnetometer normalisation)
    if((mx != 0.0f) && (my != 0.0f) && (mz != 0.0f)) {
        float hx, hy, bx, bz;
        float halfwx, halfwy, halfwz;

        // Normalise magnetometer measurement
        recipNorm = invSqrt(mx * mx + my * my + mz * mz);
        mx *= recipNorm;
        my *= recipNorm;
        mz *= recipNorm;

        // Reference direction of Earth's magnetic field
        hx = 2.0f * (mx * (0.5f - q2q2 - q3q3) + my * (q1q2 - q0q3) + mz * (q1q3 + q0q2));
        hy = 2.0f * (mx * (q1q2 + q0q3) + my * (0.5f - q1q1 - q3q3) + mz * (q2q3 - q0q1));
        bx = sqrt(hx * hx + hy * hy);
        bz = 2.0f * (mx * (q1q3 - q0q2) + my * (q2q3 + q0q1) + mz * (0.5f - q1q1 - q2q2));

        // Estimated direction of magnetic field
        halfwx = bx * (0.5f - q2q2 - q3q3) + bz * (q1q3 - q0q2);
        halfwy = bx * (q1q2 - q0q3) + bz * (q0q1 + q2q3);
        halfwz = bx * (q0q2 + q1q3) + bz * (0.5f - q1q1 - q2q2);

        // Error is sum of cross product between estimated direction and measured direction of field vectors
        halfex = (my * halfwz - mz * halfwy);
        halfey = (mz * halfwx - mx * halfwz);
        halfez = (mx * halfwy - my * halfwx);
    }

    // Compute feedback only if accelerometer measurement valid (avoids NaN in accelerometer normalisation)
    if((ax != 0.0f) && (ay != 0.0f) && (az != 0.0f))
    {
        float halfvx, halfvy, halfvz;

        // Normalise accelerometer measurement
        recipNorm = invSqrt(ax * ax + ay * ay + az * az);
        ax *= recipNorm;
        ay *= recipNorm;
        az *= recipNorm;

        // Estimated direction of gravity
        halfvx = q1q3 - q0q2;
        halfvy = q0q1 + q2q3;
        halfvz = q0q0 - 0.5f + q3q3;

        // Error is sum of cross product between estimated direction and measured direction of field vectors
        halfex += (ay * halfvz - az * halfvy);
        halfey += (az * halfvx - ax * halfvz);
        halfez += (ax * halfvy - ay * halfvx);
    }

    // Apply feedback only when valid data has been gathered from the accelerometer or magnetometer
    if(halfex != 0.0f && halfey != 0.0f && halfez != 0.0f) {
        // Compute and apply integral feedback if enabled
        if(twoKi > 0.0f)
        {
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
    gx *= (0.5f * (1.0f / sampleFreq));   // pre-multiply common factors
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

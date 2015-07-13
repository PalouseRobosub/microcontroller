#ifndef MAGNETOMETER
#define MAGNETOMETER

#include <cmath>

class Magnetometer
{
public:
    Magnetometer();
    void SetScale(float gauss);
    float alpha;
    float x, y, z;
    int x_raw, y_raw, z_raw;
    float fx, fy, fz;
    void updateMag(int x, int y, int z);

private:
    float m_scale;
};

#endif

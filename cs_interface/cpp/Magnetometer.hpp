#ifndef MAGNETOMETER
#define MAGNETOMETER

#include <cmath>
#include <cstdint>

using std::int16_t;

class Magnetometer
{
public:
    Magnetometer();
    void SetScale(float gauss);
    float alpha;
    float x, y, z;
    float fx, fy, fz;
    int16_t x_raw, y_raw, z_raw;
    void updateMag(int16_t x, int16_t y, int16_t z);

private:
    float m_scale;
};

#endif

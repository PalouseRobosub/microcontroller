#ifndef MAGNETOMETER
#define MAGNETOMETER

<<<<<<< HEAD
#include <cmath>
=======
#include <cstdint>

using std::int16_t;
>>>>>>> 295047038745c2e487f4a2f01e7a42196859dd90

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

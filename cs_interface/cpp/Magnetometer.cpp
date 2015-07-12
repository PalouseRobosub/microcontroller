#include "Magnetometer.hpp"

Magnetometer::Magnetometer()
{
    m_scale = 1.0;
}

void Magnetometer::SetScale(float gauss)
{
    if (gauss == 0.88) m_scale = 0.73;
    else if (gauss == 1.3) m_scale = 0.92;
    else if (gauss == 1.9) m_scale = 1.22;
    else if (gauss == 2.5) m_scale = 1.52;
    else if (gauss == 4.0) m_scale = 2.27;
    else if (gauss == 4.7) m_scale = 2.56;
    else if (gauss == 5.6) m_scale = 3.03;
    else if (gauss == 8.1) m_scale = 4.35;
    else m_scale = 1;
}

void Magnetometer::updateMag(int x, int y, int z)
{
    this.x_raw = x;
    this.y_raw = y;
    this.z_raw = z;

    this.x = x * m_scale;
    this.y = y * m_scale;
    this.z = z * m_scale;
}

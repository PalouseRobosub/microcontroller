#include "Magnetometer.hpp"

Magnetometer::Magnetometer()
{
    m_scale = 1.0;
    alpha = 0.5;
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

void Magnetometer::updateMag(int xm, int ym, int zm)
{
    x_raw = xm;
    y_raw = ym;
    z_raw = zm;

    fx = -x_raw*alpha + fx*(1.0-alpha);
    fy = y_raw*alpha + fy*(1.0-alpha);
    fz = -z_raw*alpha + fz*(1.0-alpha);

    //Normalize the values
    float norm = sqrt(fx*fx + fy*fy + fz*fz);
    if(norm != 0)
    {
        x = fx * m_scale / norm;
        y = fy * m_scale / norm;
        z = fz * m_scale / norm;
    }
    else
    {
        x = fx * m_scale;
        y = fy * m_scale;
        z = fz * m_scale;
    }
}

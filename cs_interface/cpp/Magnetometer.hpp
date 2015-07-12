#ifndef MAGNETOMETER
#define MAGNETOMETER

class Magnetometer
{
public:
    Magnetometer();
    void SetScale(float gauss);
    float x, y, z;
    int x_raw, y_raw, z_raw;
    void updateMag(int x, int y, int z);

private:
    float m_scale;
};

#endif

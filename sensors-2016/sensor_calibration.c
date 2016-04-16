#include "Sensors.h"
#include "sublibinal.h"

    uint8 config_mag[3] = {0};
    uint8 configuration_accel;
    uint8 config_gyro[2] = {0};
    
void configureSensors()
{
    
    switchChannel(0); //Switch to channel 0 active
    //---------------
    configureAccelerometer(0); //Configure accelerometers on Channel 0
    configureGyroscope(0);
    configureMagnometer(0);
    
    switchChannel(1); //Set channel 1 active
    //---------------
    configureAccelerometer(1); //Configure accelerometers on Channel 1
    configureGyroscope(1);
    configureMagnometer(1);
    
    //While we wait, set up the read nodes
    configureReadNodes();
}

void configureAccelerometer(int channel)
{
    configuration_accel = 1 << 3; //Turn the device on and enable reading
    
    //First, configure the Accelerometer
    I2C_Node accel_config = {0};
    accel_config.callback = NULL;
    accel_config.data_buffer = &configuration_accel;
    accel_config.data_size = 1;
    accel_config.device_address = ACCEL_ADDR;
    accel_config.mode = WRITE;
    accel_config.sub_address = 0x2D;
    
    if (channel == 0)
    {
        accel_config.device_id = SID_ACCELEROMETER_1;
        send_I2C(I2C_CH_1, accel_config);
        accel_config.device_id = SID_ACCELEROMETER_2;
        //send_I2C(I2C_CH_2, accel_config);
    }
    else if (channel == 1)
    {
        accel_config.device_id = SID_ACCELEROMETER_3;
        send_I2C(I2C_CH_1, accel_config);
    }
}

void configureGyroscope(int channel)
{
    I2C_Node gyro_config = {0};
    gyro_config.callback = NULL;
    gyro_config.data_buffer = config_gyro;
    gyro_config.data_size = 2;
    gyro_config.device_address = GYRO_ADDR;
    gyro_config.mode = WRITE;
    gyro_config.sub_address = 0x15;
    
    config_gyro[0] = 9;
    config_gyro[1] = 0x19;
    
    if (channel == 0)
    {
        gyro_config.device_id = SID_GYROSCOPE_1;
        send_I2C(I2C_CH_1, gyro_config);
        gyro_config.device_id = SID_GYROSCOPE_2;
        //send_I2C(I2C_CH_2, gyro_config);
    }
    else if (channel == 1)
    {
        gyro_config.device_id = SID_GYROSCOPE_3;
        send_I2C(I2C_CH_1, gyro_config);
    }
}

void configureMagnometer(int channel)
{
    config_mag[0] = 0b00011000; 
    config_mag[1] = 0b00100000;
    config_mag[2] = 0b00000000; //Set Magnetometer to Continuous-Measurement Mode
    
    I2C_Node mag_config = {0};
    mag_config.callback = NULL;
    mag_config.data_buffer = config_mag;
    mag_config.data_size = 3;
    mag_config.device_address = MAG_ADDR;
    mag_config.mode = WRITE;
    mag_config.sub_address = 0x00;
    
    if (channel == 0)
    {
        mag_config.device_id = SID_MAGNOMETER_1;
        send_I2C(I2C_CH_1, mag_config);
        mag_config.device_id = SID_MAGNOMETER_2;
        //send_I2C(I2C_CH_2, mag_config);
    }
    else if (channel == 1)
    {
        mag_config.device_id = SID_MAGNOMETER_3;
        send_I2C(I2C_CH_1, mag_config);
    }
}
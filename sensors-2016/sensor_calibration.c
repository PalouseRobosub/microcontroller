#include "Sensors.h"
#include "sublibinal.h"

boolean calibrationComplete = FALSE;
boolean calibrate_channel_1 = FALSE;

volatile int configurations_completed = 0;
void config_done(I2C_Node node)
{
    configurations_completed++;
    if (configurations_completed >= 6)
        calibrate_channel_1 = TRUE;
    
    if (configurations_completed >= 9) //3 accel, 3 gyro, 3 magn, 4 depth
        calibrationComplete = TRUE; //Allow our program to continue
}

void configureSensors()
{
    calibrate_channel_1 = FALSE;
    configurations_completed = 0; //Reset the configurations_completed variable
    
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
    uint8 configuration;
    configuration = 1 << 3; //Turn the device on and enable reading
    
    //First, configure the Accelerometer
    I2C_Node accel_config = {0};
    accel_config.callback = &config_done;
    accel_config.data_buffer = &configuration;
    accel_config.data_size = sizeof(configuration);
    accel_config.device_address = ACCEL_ADDR;
    accel_config.mode = WRITE;
    accel_config.sub_address = 0x2D;
    
    if (channel == 0)
    {
        accel_config.device_id = SID_ACCELEROMETER_1;
        send_I2C(I2C_CH_1, accel_config);
        accel_config.device_id = SID_ACCELEROMETER_2;
        send_I2C(I2C_CH_2, accel_config);
    }
    else if (channel == 1)
    {
        accel_config.device_id = SID_ACCELEROMETER_3;
        send_I2C(I2C_CH_1, accel_config);
    }
}

void configureGyroscope(int channel)
{
    uint8 config[2] = {0};
    I2C_Node gyro_config = {0};
    gyro_config.callback = &config_done;
    gyro_config.data_buffer = config;
    gyro_config.data_size = sizeof(config);
    gyro_config.device_address = GYRO_ADDR;
    gyro_config.mode = WRITE;
    gyro_config.sub_address = 0x15;
    
    config[0] = 100;
    config[1] = 0x9;
    
    if (channel == 0)
    {
        gyro_config.device_id = SID_GYROSCOPE_1;
        send_I2C(I2C_CH_1, gyro_config);
        gyro_config.device_id = SID_GYROSCOPE_2;
        send_I2C(I2C_CH_2, gyro_config);
    }
    else if (channel == 1)
    {
        gyro_config.device_id = SID_GYROSCOPE_3;
        send_I2C(I2C_CH_1, gyro_config);
    }
}

void configureMagnometer(int channel)
{
    uint8 config[3] = {0};
    config[0] = 0x18;
    config[1] = 0x20;
    config[2] = 0;
    
    I2C_Node mag_config = {0};
    mag_config.callback = &config_done;
    mag_config.data_buffer = &config;
    mag_config.data_size = sizeof(config);
    mag_config.device_address = MAG_ADDR;
    mag_config.mode = WRITE;
    mag_config.sub_address = 0x00;
    
    if (channel == 0)
    {
        mag_config.device_id = SID_MAGNOMETER_1;
        send_I2C(I2C_CH_1, mag_config);
        mag_config.device_id = SID_MAGNOMETER_2;
        send_I2C(I2C_CH_2, mag_config);
    }
    else if (channel == 1)
    {
        mag_config.device_id = SID_MAGNOMETER_3;
        send_I2C(I2C_CH_1, mag_config);
    }
}
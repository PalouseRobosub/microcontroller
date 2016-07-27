#include "Sensors.h"
#include "sublibinal.h"

extern I2C_Node temp_read;

void configureSensors()
{
    //Set up the read nodes
    configureReadNodes();
    
    set_mux(MUX_1_ADDR, channel_one);
    set_mux(MUX_2_ADDR, channel_none);
    configureDepth(0);
    configureAccelerometer(0); 
    configureGyroscope(0);
    configureMagnometer(0);
    set_mux(MUX_1_ADDR, channel_two);
    configureDepth(1);
    configureAccelerometer(1); 
    configureGyroscope(1);
    configureMagnometer(1);
    set_mux(MUX_1_ADDR, channel_none);
    set_mux(MUX_2_ADDR, channel_one);
    configureDepth(2);
    configureAccelerometer(2); 
    configureGyroscope(2);
    configureMagnometer(2);
    set_mux(MUX_2_ADDR, channel_two);
    configureDepth(3);
    
}

void configureAccelerometer(int channel)
{
    uint8 configuration;
    configuration = 1 << 3; //Turn the device on and enable reading
    
    //First, configure the Accelerometer
    I2C_Node accel_config = {0};
    accel_config.callback = NULL;
    accel_config.data_buffer = &configuration;
    accel_config.data_size = 1;
    accel_config.device_address = ACCEL_ADDR;
    accel_config.mode = WRITE;
    accel_config.sub_address = 0x2D;
    accel_config.device_id = SID_ACCELEROMETER_1 + channel;
    send_I2C(I2C_CH_1, accel_config);
}

void configureGyroscope(int channel)
{
    uint8 config[2] = {0};
    I2C_Node gyro_config = {0};
    uint8 config_two = 1;
    
    gyro_config.callback = NULL;
    gyro_config.data_buffer = config;
    gyro_config.data_size = 2;
    gyro_config.device_address = GYRO_ADDR;
    gyro_config.mode = WRITE;
    gyro_config.sub_address = 0x15;
    
    config[0] = 7;
    config[1] = 0x18;
    
    gyro_config.device_id = SID_GYROSCOPE_1 + channel; //channel should be 0-2
    send_I2C(I2C_CH_1, gyro_config);
}

void configureMagnometer(int channel)
{
    uint8 config[3] = {0};
    config[0] = 0b00011000; 
    config[1] = 0b00100000;
    config[2] = 0b00000000; //Set Magnetometer to Continuous-Measurement Mode
    
    I2C_Node mag_config = {0};
    mag_config.callback = NULL;
    mag_config.data_buffer = config;
    mag_config.data_size = 3;
    mag_config.device_address = MAG_ADDR;
    mag_config.mode = WRITE;
    mag_config.sub_address = 0x00;
    mag_config.device_id = SID_MAGNETOMETER_1 + channel;
    send_I2C(I2C_CH_1, mag_config);
}

void configureDepth(int channel)
{
    int i = 0;
    
    I2C_Node depth_config = {0};
    depth_config.callback = NULL;
    depth_config.data_buffer = NULL;
    depth_config.data_size = 0;
    depth_config.device_address = DEPTH_ADDR;
    depth_config.device_id = SID_DEPTH_1 + channel;
    depth_config.mode = WRITE;
    depth_config.sub_address = 0x1E; //Reset command
    
    send_I2C(I2C_CH_1, depth_config);
    queryDepthConfig(channel);
    
    temp_read.device_id = SID_TEMP_1 + channel;
    send_I2C(I2C_CH_1, temp_read);
}

void queryDepthConfig(int channel)
{
    int i = 0;
    I2C_Node depth_config = {0};
    
    //Grab depth configuration from EEPROM.
    depth_config.callback = &configRead;
    depth_config.device_address = DEPTH_ADDR;
    depth_config.mode = READ;
    depth_config.data_size = 2;
    for (i = 0; i < 6; i++)
    {
        depth_config.device_id = SID_DEPTH_CON_1_1 + channel * 6 + i;
        depth_config.sub_address = 0xA2 + i*2;
        send_I2C(I2C_CH_1, depth_config); //Read PROM
    }
}

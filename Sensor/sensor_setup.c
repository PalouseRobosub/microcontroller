#include "Sensor.h"
#include "sensor_setup.h"
#include "ADC.h"

// <editor-fold defaultstate="collapsed" desc="Global Variables">
Sensor_Data accel_data;
Sensor_Data gyro_data;
Sensor_Data mag_data;
Sensor_Data temp_data;
Sensor_Data pressure_data;
uint8 adc_work_queue[10*sizeof(ADC_Data)], adc_results_queue[10*sizeof(ADC_Data)];
// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="Local Variables">

// <editor-fold defaultstate="collapsed" desc="accelerometer variables">
I2C_Node accel_config;
uint8 accel_config_buffer[1];
I2C_Node accel_read;
uint8 accel_read_buffer[6];
// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="gyroscope variables">
I2C_Node gyro_config;
u8 gyro_config_buffer[1];
I2C_Node gyro_read;
u8 gyro_read_buffer[6];
// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="magnetometer variables">
I2C_Node mag_config;
u8 mag_config_buffer;
I2C_Node mag_read;
u8 mag_read_buffer[6];
// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="temperature sensor variables">
I2C_Node temp_config;
u8 temp_config_buffer[1];
I2C_Node temp_read;
u8 temp_read_buffer[6];
// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="pressure sensor variables">
I2C_Node pressure_config;
u8 pressure_config_buffer[1];
I2C_Node pressure_read;
u8 pressure_read_buffer[6];
// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="depth sensor variables">
ADC_Config depth_config;
ADC_Node depth_read;
// </editor-fold>

// </editor-fold>

//functions
void sensor_setup(void *callback)
{
    accel_setup(callback);
    gyro_setup(callback);
    mag_setup(callback);
    depth_setup();
}

void accel_setup(void *callback)
{
    u8 address = 0x53;

    //accelerator struct setups
    accel_config.callback = 0;
    accel_config.data_buffer = accel_config_buffer;
    accel_config.data_size = 1;
    accel_config.device_address = address;
    accel_config.sub_address = 0x2D;
    accel_config.mode = WRITE;

    accel_config_buffer[0] = (1 << 3); //turn on the accelerometer

    accel_read.callback = callback;
    accel_read.data_buffer = accel_read_buffer;
    accel_read.data_size = 6;
    accel_read.device_address = address;
    accel_read.sub_address = 0x32;
    accel_read.mode = READ;
    accel_read.device_id = SID_ACCELEROMETER_0;

    accel_data.channel = I2C_CH_1;
    accel_data.config_nodes = &accel_config;
    accel_data.config_nodes_size = 1;
    accel_data.read_nodes = &accel_read;
    accel_data.read_nodes_size = 1;
}

void gyro_setup(void *callback)
{
    u8 address = 0b1101000; //Last bit of ID is 0
    
    //gyroscope struct setups
    gyro_config.callback = 0;
    gyro_config.data_buffer = gyro_config_buffer;
    gyro_config.data_size = 2;
    gyro_config.device_address = address;
    gyro_config.sub_address = 0x15;
    gyro_config.mode = WRITE;

    gyro_config_buffer[0] = 100; //Set Sample Rate Divider to 100;
    gyro_config_buffer[1] = 0x19; //Configure Gyroscope Range and Set Internal Sample Rate

    gyro_read.callback = callback;
    gyro_read.data_buffer = gyro_read_buffer;
    gyro_read.data_size = 6;
    gyro_read.device_address = address;
    gyro_read.sub_address = 0x1D;
    gyro_read.mode = READ;
    gyro_read.device_id = SID_GYROSCOPE_0;

    gyro_data.channel = I2C_CH_1;
    gyro_data.config_nodes = &gyro_config;
    gyro_data.config_nodes_size = 1;
    gyro_data.read_nodes = &gyro_read;
    gyro_data.read_nodes_size = 1;   
}

void mag_setup(void *callback)
{
    uint8 address = 0x1E;

    //magnetometer struct setups
    mag_config.callback = 0;
    mag_config.data_buffer = &mag_config_buffer;
    mag_config.data_size = 1;
    mag_config.device_address = address;
    mag_config.sub_address = 0x02;
    mag_config.mode = WRITE;

    mag_config_buffer = 0; //Set Magnetometer to Continuous-Measurement Mode

    mag_read.callback = callback;
    mag_read.data_buffer = mag_read_buffer;
    mag_read.data_size = 6;
    mag_read.device_address = address;
    mag_read.sub_address = 0x03;
    mag_read.mode = READ;
    mag_read.device_id = SID_MAGNETOMETER_0;

    mag_data.channel = I2C_CH_1;
    mag_data.config_nodes = &mag_config;
    mag_data.config_nodes_size = 1;
    mag_data.read_nodes = &mag_read;
    mag_data.read_nodes_size = 1;
}

void depth_setup()
{
    ADC_Config adc_config = {0};

    adc_config.channels = (1 << ADC_CH_1);
    adc_config.work_buffer_ptr = adc_work_queue;
    adc_config.work_buffer_size = sizeof(adc_work_queue);
    adc_config.result_buffer_ptr = adc_results_queue;
    adc_config.result_buffer_size = sizeof(adc_results_queue);
    initialize_ADC(adc_config);
}

/*void temp_setup(void *callback)
{
    uint8 address = 0x53;

    //temperature sensor struct setups
    temp_config_node.callback = 0;
    temp_config_node.data_buffer = temp_config_buffer;
    temp_config_node.data_size = 1;
    temp_config_node.device_address = address;
    temp_config_node.sub_address = 0x2D;
    temp_config_node.mode = WRITE;

    temp_config_buffer[0] = (1 << 3); //turn on the temperature sensor

    temp_read_node.callback = callback;
    temp_read_node.data_buffer = temp_read_buffer;
    temp_read_node.data_size = 6;
    temp_read_node.device_address = address;
    temp_read_node.sub_address = 0x32;
    temp_read_node.mode = READ;
    temp_read_node.device_id = SID_TEMPERATURE_0;

    temp_data.channel = I2C_CH_1;
    temp_data.config_nodes = &temp_config_node;
    temp_data.config_nodes_size = 1;
    temp_data.read_nodes = &temp_read_node;
    temp_data.read_nodes_size = 1;
}

void pressure_setup(void *callback)
{
    uint8 address = 0x53;

    //pressure sensor struct setups
    pressure_config_node.callback = 0;
    pressure_config_node.data_buffer = pressure_config_buffer;
    pressure_config_node.data_size = 1;
    pressure_config_node.device_address = address;
    pressure_config_node.sub_address = 0x2D;
    pressure_config_node.mode = WRITE;

    pressure_config_buffer[0] = (1 << 3); //turn on the pressure sensor

    pressure_read_node.callback = callback;
    pressure_read_node.data_buffer = pressure_read_buffer;
    pressure_read_node.data_size = 6;
    pressure_read_node.device_address = address;
    pressure_read_node.sub_address = 0x32;
    pressure_read_node.mode = READ;
    pressure_read_node.device_id = SID_PRESSURE_0;

    pressure_data.channel = I2C_CH_1;
    pressure_data.config_nodes = &pressure_config_node;
    pressure_data.config_nodes_size = 1;
    pressure_data.read_nodes = &pressure_read_node;
    pressure_data.read_nodes_size = 1;
}*/

void config_accel(void) 
{
    initialize_sensor(accel_data);
}

void read_accel(void)
{
    read_sensor(accel_data);
}

void config_gyro(void)
{
    initialize_sensor(gyro_data);
}

void read_gyro(void)
{
    uint8 temp;
    read_sensor(gyro_data);
    temp = gyro_read.data_buffer[0];
    gyro_read.data_buffer[0] = gyro_read.data_buffer[1];
    gyro_read.data_buffer[1] = temp;

    temp = gyro_read.data_buffer[2];
    gyro_read.data_buffer[2] = gyro_read.data_buffer[3];
    gyro_read.data_buffer[3] = temp;

    temp = gyro_read.data_buffer[4];
    gyro_read.data_buffer[4] = gyro_read.data_buffer[5];
    gyro_read.data_buffer[5] = temp;

}

void config_mag(void)
{
    initialize_sensor(mag_data);
}

void read_mag(void)
{
    read_sensor(mag_data);

    uint8 temp;
    read_sensor(mag_data);
    temp = mag_read.data_buffer[0];
    mag_read.data_buffer[0] = mag_read.data_buffer[1];
    mag_read.data_buffer[1] = temp;

    temp = mag_read.data_buffer[2];
    mag_read.data_buffer[2] = mag_read.data_buffer[5];
    mag_read.data_buffer[5] = temp;

    temp = mag_read.data_buffer[4];
    mag_read.data_buffer[4] = mag_read.data_buffer[3];
    mag_read.data_buffer[3] = temp;
}








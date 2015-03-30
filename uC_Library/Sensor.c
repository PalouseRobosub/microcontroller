
#include "Sensor.h"

//functions
void initialize_sensor(Sensor_Data data)
{
    int i;
    
    switch (data.sensor_type)
    {
        case I2C_SENSOR:
            for (i=0; i<data.i2c_data.config_nodes_size; ++i)
            {
                send_I2C(data.i2c_data.channel, data.i2c_data.config_nodes[i]);
            }

            break;

        //sensors that don't need configuration
        case ADC_SENSOR:
            break;
    }
}

void read_sensor(Sensor_Data data)
{
    int i;
    
    switch (data.sensor_type)
    {
        case I2C_SENSOR:
            for (i=0; i<data.i2c_data.read_nodes_size; ++i)
            {
                send_I2C(data.i2c_data.channel, data.i2c_data.read_nodes[i]);
            }
            break;

        case ADC_SENSOR:
            for(i=0; i<data.adc_data.adc_node_size; ++i)
            {
                read_ADC(data.adc_data.adc_node[i]);
            }
            break;
    }
}




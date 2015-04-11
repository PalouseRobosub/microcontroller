
#include "Sensor.h"

//functions
void initialize_sensor(Sensor_Data data)
{
    int i;
    
    for (i=0; i<data.config_nodes_size; ++i)
    {
        send_I2C(data.channel, data.config_nodes[i]);
    }
}

void read_sensor(Sensor_Data data)
{
    int i;    

    for (i=0; i<data.read_nodes_size; ++i)
    {
        send_I2C(data.channel, data.read_nodes[i]);
    }
}




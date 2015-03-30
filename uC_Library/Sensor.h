/* 
 * File:   Sensor.h
 * Author: James Irwin
 *
 * Created on March 29, 2015, 6:04 PM
 */

#ifndef SENSOR_H
#define	SENSOR_H

#ifdef	__cplusplus
extern "C" {
#endif
    
/*Include Statements*/
#include "System.h"
#include "ADC.h"
#include "I2C.h"


    typedef enum {
        I2C_SENSOR,
        ADC_SENSOR
    }Sensor_Type;

    typedef struct SENSOR_DATA_I2C {
        I2C_Channel channel;
        I2C_Node *config_nodes;
        uint8 config_nodes_size;
        I2C_Node *read_nodes;
        uint8 read_nodes_size;
    } Sensor_Data_I2C;

    typedef struct SENSOR_DATA_ADC {
        ADC_Node *adc_node;
        uint8 adc_node_size;
    } Sensor_Data_ADC;

    typedef struct SENSOR_DATA{
        Sensor_Type sensor_type;
        Sensor_Data_I2C i2c_data;
        Sensor_Data_ADC adc_data;
        void (*callback) (struct SENSOR_DATA);
    } Sensor_Data;

    void initialize_sensor(Sensor_Data data);
    void read_sensor(Sensor_Data data);




#ifdef	__cplusplus
}
#endif

#endif	/* SENSOR_H */


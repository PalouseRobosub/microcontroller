/* 
 * File:   sensor_ids.h
 * Author: james
 *
 * Created on April 11, 2015, 5:37 PM
 */

#ifndef SENSOR_IDS_H
#define	SENSOR_IDS_H

#ifdef	__cplusplus
extern "C" {
#endif

typedef enum SENSOR_ID
{
    SID_ACCELEROMETER_0,
    SID_GYROSCOPE_0,
    SID_MAGNETOMETER_0,
    SID_TEMPERATURE_0,
    SID_PRESSURE_0,
    SID_DEPTH_0,
    SID_START_SWITCH,

    SID_COUNT
} Sensor_ID;

void sensor_setup(void *callback);
void accel_setup(void *callback);
void gyro_setup(void *callback);
void mag_setup(void *callback);
void depth_setup();

void config_accel(void);
void read_accel(void);
void config_gyro(void);
void read_gyro(void);
void config_mag(void);
void read_mag(void);

#ifdef	__cplusplus
}
#endif

#endif	/* SENSOR_IDS_H */


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

typedef enum SENSOR_ID {
    SID_ACCELEROMETER_0,
    SID_GYROSCOPE_0,
    SID_MAGNOMETER_0,
    SID_TEMPERATURE_0,

    SID_COUNT
    } Sensor_ID;


#ifdef	__cplusplus
}
#endif

#endif	/* SENSOR_IDS_H */


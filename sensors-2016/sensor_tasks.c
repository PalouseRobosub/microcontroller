#include "Sensors.h"

extern I2C_Node gyro_read, mag_read, accel_read, depth_read, depth_prep, temp_read, temp_prep;

/**
 * Enqueues a read gyroscope and accelerometer Task.
 *
 * @brief This will enqueue 11 I2C nodes.
 * 
 * @note This function will enqueue a sequence of i2c
 *       transactions that will provide all data from
 *       the accelerometer and gyroscopes. The final
 *       read will have a callback to signal that all
 *       data has been acquired and can be transmitted.
 * 
 * @note This function is inlined to save time in an ISR.
 */
inline void __attribute__((always_inline)) gyroscope_accelerometer_read_task()
{
    set_mux(MUX_1_ADDR, channel_one);
    set_mux(MUX_2_ADDR, channel_none);
    send_I2C(I2C_CH_1, gyro_read);
    send_I2C(I2C_CH_1, accel_read);
    set_mux(MUX_1_ADDR, channel_two);
    send_I2C(I2C_CH_1, gyro_read);
    send_I2C(I2C_CH_1, accel_read);
    set_mux(MUX_1_ADDR, channel_none);
    set_mux(MUX_2_ADDR, channel_one);
    send_I2C(I2C_CH_1, gyro_read);
    send_I2C(I2C_CH_1, accel_read);
}
/**
 * Enqueues a magnetometer read task onto the I2C interface.
 *
 * @brief This will enqueue 8 I2C nodes.
 *
 * @note This function enqueues the steps necessary to read
 *       all four of the magnetometers.
 *
 * @note This function is always inlined to reduce latency for calling
 *       within ISRs
 *
 * @return None.
 */
inline void __attribute__((always_inline)) magnetometer_read_task()
{
    set_mux(MUX_1_ADDR, channel_one);
    set_mux(MUX_2_ADDR, channel_none);
    send_I2C(I2C_CH_1, mag_read);
    set_mux(MUX_1_ADDR, channel_two);
    send_I2C(I2C_CH_1, mag_read);
    set_mux(MUX_1_ADDR, channel_none);
    set_mux(MUX_2_ADDR, channel_one);
    send_I2C(I2C_CH_1, mag_read);
}
/**
 * This enqueues a depth read task and also begins a new depth conversion.
 * 
 * @brief This will enqueue 14 I2C nodes.
 * 
 * @note This function enqueues all of the steps necessary to read all of
 *       the present depth sensors. It will then also force a new conversion
 *       to start on each sensor that it reads.
 * 
 * @note This function is always inlined to reduce latency for calling
 *       within ISRs.
 * 
 * @return None.
 */
inline void __attribute__((always_inline)) depth_read_depth_prep_task()
{
    set_mux(MUX_1_ADDR, channel_one);
    set_mux(MUX_2_ADDR, channel_none);
    send_I2C(I2C_CH_1, depth_read);
    send_I2C(I2C_CH_1, depth_prep);
    set_mux(MUX_1_ADDR, channel_two);
    send_I2C(I2C_CH_1, depth_read);
    send_I2C(I2C_CH_1, depth_prep);
    set_mux(MUX_1_ADDR, channel_none);
    set_mux(MUX_2_ADDR, channel_one);
    send_I2C(I2C_CH_1, depth_read);
    send_I2C(I2C_CH_1, depth_prep);
    set_mux(MUX_2_ADDR, channel_two);
    send_I2C(I2C_CH_1, depth_read);
    send_I2C(I2C_CH_1, depth_prep);
}

/**
 * Reads the current depth measurement waiting and initiates a temperature
 * conversion.
 *
 * @brief This function enqueues 14 I2C nodes.
 * 
 * @note This function is always inlined to reduce latency when called within
 *       ISRs.
 * 
 * @return None.
 */
inline void __attribute__((always_inline)) depth_read_temp_prep_task()
{
    set_mux(MUX_1_ADDR, channel_one);
    set_mux(MUX_2_ADDR, channel_none);
    send_I2C(I2C_CH_1, depth_read);
    send_I2C(I2C_CH_1, temp_prep);
    set_mux(MUX_1_ADDR, channel_two);
    send_I2C(I2C_CH_1, depth_read);
    send_I2C(I2C_CH_1, temp_prep);
    set_mux(MUX_1_ADDR, channel_none);
    set_mux(MUX_2_ADDR, channel_one);
    send_I2C(I2C_CH_1, depth_read);
    send_I2C(I2C_CH_1, temp_prep);
    set_mux(MUX_2_ADDR, channel_two);
    send_I2C(I2C_CH_1, depth_read);
    send_I2C(I2C_CH_1, temp_prep);
}
/**
 * Enqueues a temperature read and initializes a depth conversion.
 * 
 * @brief This function enqueues 14 I2C nodes.
 * 
 * @brief This function is always inlined to reduce latency when called within
 *        ISRs.
 * 
 * @return None.
 */
inline void __attribute__((always_inline)) temp_read_depth_prep_task()
{
    set_mux(MUX_1_ADDR, channel_one);
    set_mux(MUX_2_ADDR, channel_none);
    send_I2C(I2C_CH_1, temp_read);
    send_I2C(I2C_CH_1, depth_prep);
    set_mux(MUX_1_ADDR, channel_two);
    send_I2C(I2C_CH_1, temp_read);
    send_I2C(I2C_CH_1, depth_prep);
    set_mux(MUX_1_ADDR, channel_none);
    set_mux(MUX_2_ADDR, channel_one);
    send_I2C(I2C_CH_1, temp_read);
    send_I2C(I2C_CH_1, depth_prep);
    set_mux(MUX_2_ADDR, channel_two);
    send_I2C(I2C_CH_1, temp_read);
    send_I2C(I2C_CH_1, depth_prep);
}
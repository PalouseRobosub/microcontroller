#include "Sensors.h"

/**
 * Sets a mux to a specific channel.
 *
 * @param i2c_address The address of the mux chip.
 *
 * @param channel The mux channel to set the chip to enable.
 *
 * @return None.
 */
void set_mux(uint8 i2c_address, Mux_Channel channel)
{
    I2C_Node mux;
    mux.callback = NULL;
    mux.data_buffer = NULL;
    mux.device_address = i2c_address;
    mux.data_size = 0;
    mux.device_id = NULL;
    mux.mode = WRITE;
    switch (channel)
    {
        default:
        case channel_none:
            mux.sub_address = 0;
            break;
        case channel_one:
            mux.sub_address = 1;
            break;
        case channel_two:
            mux.sub_address = 0b10;
            break;
    }
    
    send_I2C(I2C_CH_1, mux);
}
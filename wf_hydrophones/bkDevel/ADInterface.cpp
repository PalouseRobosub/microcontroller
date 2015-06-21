#include "hydrophones.h"

/* Function: enumDevices () 
 * Description: Finds the two devices specified by AD1 and AD2
 * Input Params: None
 * Returns: A vector<int> in the form [AD1_loc, AD2_loc]
 * Preconditions: Program started
 * Postconditions: If a device is not found -1 will be returned as the device location
 */
vector<int> enumDevs()
{
    int num_devs = 0;
    vector<int> devs(2,-1);
    char enum_SN[5][32];

    //Get the number of connected devices
    //Using enumfilterDiscovery assuming all connected devices are Analog Discoveries
    FDwfEnum(enumfilterDiscovery, &num_devs);

    //Show number of detected devices
    printf("Found %d Analog Discoveries.\n", num_devs);

    //find the SN for each device
    for(int i = 0; i < num_devs; ++i)
    {
        FDwfEnumSN(i, enum_SN[i]);
    }

    //print the SNs
    for(int i = 0; i < num_devs; ++i)
    {
        if (strcmp(enum_SN[i], AD1) == 0)
        {
            cout << "Found AD1" << endl;
            devs[0] = i;
        }
        else if (strcmp(enum_SN[i], AD2) == 0)
        {
            cout << "Found AD2" << endl;
            devs[1] = i;
        }
    }

    return devs;
}

/* Function: openDevs ()
 * Description: Opens two devices so they can be used
 * Input Params: The two devices to open through their enumerated index (int)
 * Return Params: The handles (vector<HDWF> *) for the devices
 * Preconditions: Program started
 * Postconditions: Devices can be setup for specific tasks and used
 */
void openDevs(int dev1, int dev2, vector<HDWF> *handles)
{
    if (dev1 != -1)
    {
        !FDwfDeviceOpen(dev1, &((*handles)[0])) ? cout << "AD1 could not be opened!" << endl : cout << "AD1 opened" << endl;
    } else cout << "AD1 is not connected!" << endl;
    if (dev2 != -1)
    {
        !FDwfDeviceOpen(dev2, &((*handles)[1])) ? cout << "AD2 could not be opened!" << endl : cout << "AD2 opened" << endl;
    } else cout << "AD2 is not connected!" << endl;
}

/* Function: analogReadDev ()
 * Description: Reads the analog voltage on the given channel on the given device
 * Input Params: The device handle (HDWF) and the channel (int) on the given device to read from
 * Returns: The analog voltage (double)
 * Preconditions: Devices opened via openDevs and device setup to read analog via setupAnalogRead
 * Postconditions: None
 */
double analogReadDev(HDWF handle, int channel)
{
    double voltage;
    FDwfAnalogInStatus(handle, false, NULL);

    FDwfAnalogInStatusSample(handle, channel, &voltage);

    return voltage;
}

/* Function: setupAnalogRead ()
 * Description: Sets up the analog channels on a given device for reading
 * Input Params: The device handle (HDWF), the channel (int) on the given device to set up, the range (double) of voltage that will be input both positive and negative (i.e. 5 will set up a range from -2.5V to 2.5V) and the offset (double) of voltage for the channel from zero.
 * Returns: Nothing
 * Preconditions: Devices opened via openDevs
 * Postconditions: Given device can now be read from
 */
void setupAnalogRead(HDWF handle, int channel, double range, double offset)
{
    FDwfAnalogInReset(handle);
    FDwfAnalogInFrequencySet(handle, 1000000);
    FDwfAnalogInChannelOffsetSet(handle, channel, offset);

    double actualOffset;

    FDwfAnalogInChannelOffsetGet(handle, channel, &actualOffset);

    cout << "Offset set to: " << actualOffset << endl;

    // set 5V pk2pk input range, -2.5V to 2.5V
    FDwfAnalogInChannelRangeSet(handle, channel, range);

    double actualRange;

    FDwfAnalogInChannelRangeGet(handle, channel, &actualRange);

    cout << "Range set to: " << actualRange << endl;
    // start signal generation
    //Do we want to reset the auto trigger timeout? y = set p3 to true
    FDwfAnalogInConfigure(handle, channel, false);
}

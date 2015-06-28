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
    vector<int> devs(2, -1);
    char enum_SN[5][32];

    //Get the number of connected devices
    //Using enumfilterDiscovery assuming all connected devices are Analog Discoveries
    FDwfEnum(enumfilterDiscovery, &num_devs);

    //Show number of detected devices
    printf("Found %d Analog Discoveries.\n", num_devs);

    //find the SN for each device
    for (int i = 0; i < num_devs; ++i)
    {
        FDwfEnumSN(i, enum_SN[i]);
    }

    //print the SNs
    for (int i = 0; i < num_devs; ++i)
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
void openDevs(int dev, HDWF * handle)
{
    if (dev != -1)
    {
        !FDwfDeviceOpen(dev, handle) ? cout << "Device could not be opened!" << endl : cout << "Device opened" << endl;
    }
    else cout << "Device could not be connected!" << endl;
}

/* Function: analogReadSingleDataDev ()
 * Description: Reads the analog voltage on the given channel on the given device
 * Input Params: The device handle (HDWF) and the channel (int) on the given device to read from
 * Returns: The analog voltage (double)
 * Preconditions: Devices opened via openDevs and device setup to read analog via setupAnalogRead
 * Postconditions: None
 */
double analogReadSingleDataDev(HDWF handle, int channel)
{
    double voltage;
    FDwfAnalogInStatus(handle, false, NULL);

    FDwfAnalogInStatusSample(handle, channel, &voltage);

    return voltage;
}

//TODO: Add more guard code
/* Function: setupAnalogRead ()
 * Description: Sets up the analog channels on a given device for reading
 * Input Params: The device handle (HDWF), the channel (int) on the given device to set up, the range (double) of voltage that will be input both positive and negative (i.e. 5 will set up a range from -2.5V to 2.5V) and the offset (double) of voltage for the channel from zero.
 * Returns: Nothing
 * Preconditions: Devices opened via openDevs
 * Postconditions: Given device can now be read from
 */
void setupAnalogRead(HDWF handle, bool ch1, bool ch2, double range, double offset)
{
    FDwfAnalogInReset(handle);
    FDwfAnalogInFrequencySet(handle, 1000000);

    //Set the offset for the desired channels
    if (ch1)
    {
        FDwfAnalogInChannelOffsetSet(handle, 0, offset);

        double actualOffset;

        FDwfAnalogInChannelOffsetGet(handle, 0, &actualOffset);
        cout << "CH1 offset set to: " << actualOffset << endl;
    }
    if (ch2)
    {
        FDwfAnalogInChannelOffsetSet(handle, 1, offset);
        double actualOffset;

        FDwfAnalogInChannelOffsetGet(handle, 1, &actualOffset);
        cout << "CH2 offset set to: " << actualOffset << endl;
    }

    //Set the range for the desired channels
    if (ch1)
    {
        FDwfAnalogInChannelRangeSet(handle, 0, range);

        double actualRange;

        FDwfAnalogInChannelRangeGet(handle, 0, &actualRange);

        cout << "CH1 range set to: " << actualRange << endl;
    }
    if (ch2)
    {
        FDwfAnalogInChannelRangeSet(handle, 1, range);

        double actualRange;

        FDwfAnalogInChannelRangeGet(handle, 1, &actualRange);

        cout << "CH2 range set to: " << actualRange << endl;
    }

    // start signal generation
    //Do we want to reset the auto trigger timeout? y = set p3 to true
    FDwfAnalogInConfigure(handle, false, false);
}

//TODO: Add more guard code
/* Function: setupRecordAnalogRead ()
 * Description: Sets up the analog channels on a given device for reading
 * Input Params: The device handle (HDWF), the channel (int) on the given device to set up, the range (double) of voltage that will be input both positive and negative (i.e. 5 will set up a range from -2.5V to 2.5V) and the offset (double) of voltage for the channel from zero.
 * Returns: Nothing
 * Preconditions: Devices opened via openDevs
 * Postconditions: Given device can now be read from
 * If wanting to acquire continuous results pass in -1 for the sample size
 */
void setupRecordAnalogRead(HDWF handle, bool ch1, bool ch2, double range, double offset, double freq, int sample_size)
{
    FDwfAnalogInReset(handle);
    FDwfAnalogInFrequencySet(handle, 1000000);

    //Set offset for channel 1
    if (ch1)
    {
        FDwfAnalogInChannelOffsetSet(handle, 0, offset);

        double actualOffset;

        FDwfAnalogInChannelOffsetGet(handle, 0, &actualOffset);

        cout << "CH1 offset set to: " << actualOffset << endl;
    }

    //Set offset for channel 2
    if (ch2)
    {
        FDwfAnalogInChannelOffsetSet(handle, 1, offset);

        double actualOffset;

        FDwfAnalogInChannelOffsetGet(handle, 1, &actualOffset);

        cout << "CH2 offset set to: " << actualOffset << endl;
    }

    //Set range for channel 1
    if (ch1)
    {
        FDwfAnalogInChannelRangeSet(handle, 0, range);

        double actualRange;

        FDwfAnalogInChannelRangeGet(handle, 0, &actualRange);

        cout << "CH1 range set to: " << actualRange << endl;
    }

    //Set range for channel 2
    if (ch2)
    {
        FDwfAnalogInChannelRangeSet(handle, 1, range);

        double actualRange;

        FDwfAnalogInChannelRangeGet(handle, 1, &actualRange);

        cout << "CH2 range set to: " << actualRange << endl;
    }

    //Set the acquisition mode to record
    FDwfAnalogInAcquisitionModeSet(handle, acqmodeRecord);

    //Set Frequency as desired
    FDwfAnalogInFrequencySet(handle, freq);

    //If continuous record is not wanted
    if (sample_size != -1)
    {
        FDwfAnalogInRecordLengthSet(handle, 1.0*sample_size / freq);
    }
    //otherwise
    else
    {
        FDwfAnalogInRecordLengthSet(handle, -1.0);
    }

    //Begin recording
    FDwfAnalogInConfigure(handle, false, true);
}

//Pass the device hardware definition as an argument
//This is the thread that will read data from a device
//TODO: Determine best way to read the devices in parallel
void *readDevice(void * arg)
{
    if (arg == NULL) return (void*)-1; //TODO: Is this right?

    while (true)
    {
        //update a buffer with the latest set of data
    }
}

//TODO: Add cross correlation threading

//TODO: Develop a main that manages the data passing

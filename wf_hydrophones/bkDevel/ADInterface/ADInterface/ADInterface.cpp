#include "sys.h"

//?TODO?: enumDevs()
//        Add ability to pass in array/vector of SNs to find and return their locations
//        This will lead to a slight, one time performance hit due to more looping being needed

/* Function: enumDevices ()
 * Description: Finds the two devices specified by AD1 and AD2
 * Input Params: None
 * Returns: A vector<int> in the form [AD1_loc, AD2_loc]
 * Preconditions: Program started
 * Postconditions: If a device is not found -1 will be returned as the device location
 * Currently only supports up to 5 analog discovery devices connected
 * Will not find Electronics Explorer boards!!!
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
    #if DEBUG
        cout << "Found " << num_devs << " Analog Discoveries."<< endl;
    #endif

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
            #if DEBUG
                cout << "Found AD1" << endl;
            #endif
            devs[0] = i;
        }
        else if (strcmp(enum_SN[i], AD2) == 0)
        {
            #if DEBUG
                cout << "Found AD2" << endl;
            #endif
            devs[1] = i;
        }
        else if (DEBUG)
        {
            cout << "Found device: " << enum_SN[i] << endl;
        }
    }

    return devs;
}

/* Function: openDev ()
 * Description: Open a device so they can be used
 * Input Params: The device to open through its enumerated index (int)
 * Return Params: The handle (HDWF *) for the device
 * Preconditions: Program started
 * Postconditions: Devices can be setup for specific tasks and used
 */
void openDev(int dev, HDWF * handle)
{
    if (dev != -1)
    {
        if(!FDwfDeviceOpen(dev, handle))
        {
          char szError[512];
          FDwfGetLastErrorMsg(szError);
          cout << "Error in FDwfDeviceOpen(): " << szError << endl;
          return;
        }
        else if(DEBUG) cout << "Device " << dev << " opened" << endl;
    }
    else cout << "Device " << dev << " could not be connected (Invalid Device Number)!" << endl;
}

/* Function: analogReadSingleDataDev ()
 * Description: Reads the analog voltage on the given channel on the given device
 * Input Params: device handle (HDWF)
 *               channel (int)
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

/* Function: setupAnalogRead ()
 * Description: Sets up the analog channels on a given device for reading
 * Input Params: device handle (HDWF)
 *               ch1 (bool) set true if channel 1 is to be used
 *               ch2 (bool) set true if channel 2 is to be used
 *               range (double)
 *               offset (double)
 *               freq (double) - frequency to record at
 * Returns: Nothing
 * Preconditions: Devices opened via openDevs
 * Postconditions: Given device can now be read from
 */
void setupAnalogRead(HDWF handle, bool ch1, bool ch2, double range, double offset, double freq)
{
    if(!FDwfAnalogInReset(handle))
    {
      char szError[512];
      FDwfGetLastErrorMsg(szError);
      cout << "Error in FDwfAnalogInReset(): " << szError << endl;
      return;
    }

    if(!FDwfAnalogInFrequencySet(handle, freq))
    {
      char szError[512];
      FDwfGetLastErrorMsg(szError);
      cout << "Error in FDwfAnalogInFrequencySet(): " << szError << endl;
      return;
    }

    if (ch1)
    {
        //Set the offset for the desired channels
        if(!FDwfAnalogInChannelOffsetSet(handle, 0, offset))
        {
          char szError[512];
          FDwfGetLastErrorMsg(szError);
          cout << "Error in FDwfAnalogInChannelOffsetSet(): " << szError << endl;
          return;
        }

        double actualOffset;

        FDwfAnalogInChannelOffsetGet(handle, 0, &actualOffset);
        #if DEBUG
            cout << "CH1 offset set to: " << actualOffset << endl;
        #endif

        //Set the range for the desired channels
        if(!FDwfAnalogInChannelRangeSet(handle, 0, range))
        {
          char szError[512];
          FDwfGetLastErrorMsg(szError);
          cout << "Error in FDwfAnalogInChannelRangeSet(): " << szError << endl;
          return;
        }

        double actualRange;

        FDwfAnalogInChannelRangeGet(handle, 0, &actualRange);
        #if DEBUG
            cout << "CH1 range set to: " << actualRange << endl;
        #endif
    }
    if (ch2)
    {
        //Set the offset for the desired channels
        if(!FDwfAnalogInChannelOffsetSet(handle, 1, offset))
        {
          char szError[512];
          FDwfGetLastErrorMsg(szError);
          cout << "Error in FDwfAnalogInChannelOffsetSet(): " << szError << endl;
          return;
        }
        double actualOffset;

        FDwfAnalogInChannelOffsetGet(handle, 1, &actualOffset);
        #if DEBUG
            cout << "CH2 offset set to: " << actualOffset << endl;
        #endif

        //Set the range for the desired channels
        if(!FDwfAnalogInChannelRangeSet(handle, 1, range))
        {
          char szError[512];
          FDwfGetLastErrorMsg(szError);
          cout << "Error in FDwfAnalogInChannelRangeSet(): " << szError << endl;
          return;
        }

        double actualRange;

        FDwfAnalogInChannelRangeGet(handle, 1, &actualRange);
        #if DEBUG
            cout << "CH2 range set to: " << actualRange << endl;
        #endif
    }

    // start signal capture
    FDwfAnalogInConfigure(handle, false, false);
}

/* Function: setupRecordAnalogRead ()
 * Description: Sets up the analog channels on a given device for reading
 * Input Params: device handle (HDWF)
 *               ch1 (bool) - set true if channel 1 is to be used
 *               ch2 (bool) - set true if channel 2 is to be used
 *               range (double)
 *               offset (double)
 *               freq (double) - frequency to record at
 *               sample_size (int) - number of samples to collect
 * Returns: Nothing
 * Preconditions: Devices opened via openDevs
 * Postconditions: Given device can now be read from
 * If wanting to acquire continuous results pass in -1 for the sample size
 */
void setupRecordAnalogRead(HDWF handle, bool ch1, bool ch2, double range, double offset, double freq, int sample_size)
{
    //Reset the device
    if(!FDwfAnalogInReset(handle))
    {
      char szError[512];
      FDwfGetLastErrorMsg(szError);
      cout << "Error in FDwfAnalogInReset(): " << szError << endl;
      return;
    }

    //Set the sample frequency
    if(!FDwfAnalogInFrequencySet(handle, freq))
    {
      char szError[512];
      FDwfGetLastErrorMsg(szError);
      cout << "Error in FDwfAnalogInFrequencySet(): " << szError << endl;
      return;
    }

    if (ch1)
    {
        //Enable the channel
        if(!FDwfAnalogInChannelEnableSet(handle, 0, true))
        {
          char szError[512];
          FDwfGetLastErrorMsg(szError);
          cout << "Error in FDwfAnalogInChannelEnableSet(): " << szError << endl;
          return;
        }

        //Set offset for channel 1
        if(!FDwfAnalogInChannelOffsetSet(handle, 0, offset))
        {
          char szError[512];
          FDwfGetLastErrorMsg(szError);
          cout << "Error in FDwfAnalogInChannelOffsetSet(): " << szError << endl;
          return;
        }
        double actualOffset;
        FDwfAnalogInChannelOffsetGet(handle, 0, &actualOffset);
        #if DEBUG
            cout << "CH1 offset set to: " << actualOffset << endl;
        #endif

        //Set range for channel 1
        if(!FDwfAnalogInChannelRangeSet(handle, 0, range))
        {
          char szError[512];
          FDwfGetLastErrorMsg(szError);
          cout << "Error in FDwfAnalogInChannelRangeSet(): " << szError << endl;
          return;
        }
        double actualRange;
        FDwfAnalogInChannelRangeGet(handle, 0, &actualRange);
        #if DEBUG
            cout << "CH1 range set to: " << actualRange << endl;
        #endif
    }


    if (ch2)
    {
        //Enable the channel
        if(!FDwfAnalogInChannelEnableSet(handle, 1, true))
        {
          char szError[512];
          FDwfGetLastErrorMsg(szError);
          cout << "Error in FDwfAnalogInChannelEnableSet(): " << szError << endl;
          return;
        }

        //Set offset for channel 2
        if(!FDwfAnalogInChannelOffsetSet(handle, 1, offset))
        {
          char szError[512];
          FDwfGetLastErrorMsg(szError);
          cout << "Error in FDwfAnalogInChannelOffsetSet(): " << szError << endl;
          return;
        }
        double actualOffset;
        FDwfAnalogInChannelOffsetGet(handle, 1, &actualOffset);
        #if DEBUG
            cout << "CH2 offset set to: " << actualOffset << endl;
        #endif

        //Set range for channel 2
        if(!FDwfAnalogInChannelRangeSet(handle, 1, range))
        {
          char szError[512];
          FDwfGetLastErrorMsg(szError);
          cout << "Error in FDwfAnalogInChannelRangeSet(): " << szError << endl;
          return;
        }
        double actualRange;
        FDwfAnalogInChannelRangeGet(handle, 1, &actualRange);
        #if DEBUG
            cout << "CH2 range set to: " << actualRange << endl;
        #endif
    }

    //Set the acquisition mode to record
    if(!FDwfAnalogInAcquisitionModeSet(handle, acqmodeRecord))
    {
      char szError[512];
      FDwfGetLastErrorMsg(szError);
      cout << "Error in FDwfAnalogInAcquisitionModeSet(): " << szError << endl;
      return;
    }

    //If continuous record is not wanted
    if (sample_size != -1)
    {
        if(!FDwfAnalogInRecordLengthSet(handle, 1.0*sample_size / freq))
        {
          char szError[512];
          FDwfGetLastErrorMsg(szError);
          cout << "Error in FDwfAnalogInRecordLengthSet(): " << szError << endl;
          return;
        }
    }
    //otherwise
    else
    {
        if(!FDwfAnalogInRecordLengthSet(handle, -1.0))
        {
          char szError[512];
          FDwfGetLastErrorMsg(szError);
          cout << "Error in FDwfAnalogInRecordLengthSet(): " << szError << endl;
          return;
        }
    }
}

/* Function: beginRecord ()
 * Description: Starts recording for a specific device
 * Input Params: The hardware handle for a device
 * Returns: Nothing
 * Preconditions: Device opened via openDevs
 * Postconditions: Given Device starts collection of data
 */
void beginRecord(HDWF handle)
{
    FDwfAnalogInConfigure(handle, false, true);
}

string getFileName(int threadNum, int CHNum, int sampleNum)
{
    return NULL;
}

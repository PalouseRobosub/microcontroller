#include "hydrophones.h"
#include <pthread.h> //For threading

/*  General DWF Lib usage

1. Call enumeration functions to discover connected hardware devices.
2. Call FDwfDeviceOpen function to establish a connection to specific hardware device.
3. Call function to enable instrument within hardware device.
4. Call functions to configure instrument and acquire/generate signals.
5. Call function to disable instrument.
6. Call FDwfDeviceClose function to disconnect from device

*/

//Global test variables. Will later be moved.
double frequency = 300000.0;
int samplesPerBuf = frequency * 2;

//Global Variables to manage threading
pthread_t threadId[3];

int main(int carg, char **szarg){
    vector<int> cDevice;
    vector<HDWF> hdwfs(2,-1);

    //Enumerate the Devices
    cDevice = enumDevs();
    cout << "AD1 is at: " << cDevice[0] << endl;
    cout << "AD2 is at: " << cDevice[1] << endl;

    openDev(cDevice[0], &(hdwfs[0]));
    openDev(cDevice[1], &(hdwfs[1]));
    cout << hdwfs.size() << endl;
    cout << "openDevs() returned [" << (hdwfs)[0] << ", " << (hdwfs)[1] << "]" << endl;

    //Open the devices to record with
    if (hdwfs[0] != -1)
    {
        setupRecordAnalogRead(hdwfs[0], true, true, 5, 0, frequency, samplesPerBuf);
    }
    if (hdwfs[1] != -1)
    {
        setupRecordAnalogRead(hdwfs[1], true, true, 5, 0, frequency, samplesPerBuf);
    }
    // wait at least 2 seconds with Analog Discovery for the offset to stabilize, before the first reading after device open or offset/range change
    Wait(2);

    //TODO: Add thread inits here
    for (int i = 0; i < 2; ++i)
    {
      int err = pthread_create(&(threadId[i]), NULL, &readDevice, (void *)hdwfs[i]);
      if (err != 0) cout << "Thread " << i << " could not be created: " << strerror(err) << endl;
      else cout << "Thread " << i << " created." << endl;
    }

    //TODO: Start Cross Correlation Thread
    int err = pthread_create(&(threadId[2]), NULL, &crossCorrelation, NULL);
    if (err != 0) cout << "Cross Correlation Thread was not created: " << strerror(err) << endl;
    else cout << "Cross Correlation Thread Running." << endl;

    while(true)
    {
      //TODO: Manage the threads here to search for exit status
    }
    // before application exit make sure to close all opened devices by this process
    FDwfDeviceCloseAll();

    return 0;
}

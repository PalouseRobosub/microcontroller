#include "hydrophones.h"
#include <ctime>

/*  General DWF Lib usage

1. Call enumeration functions to discover connected hardware devices.
2. Call FDwfDeviceOpen function to establish a connection to specific hardware device.
3. Call function to enable instrument within hardware device.
4. Call functions to configure instrument and acquire/generate signals.
5. Call function to disable instrument.
6. Call FDwfDeviceClose function to disconnect from device

*/

int main(int carg, char **szarg){
    vector<int> cDevice;
    vector<HDWF> hdwfs(2,-1);
    double v11 = -10;
    double v12 = -10;
    double v21 = -10;
    double v22 = -10;

    vector<double> datas1;
    vector<double> datas2;
    vector<double> datas3;
    vector<double> datas4;
    vector<double> errs;

    //Enumerate the Devices
    cDevice = enumDevs();
    cout << "AD1 is at: " << cDevice[0] << endl;
    cout << "AD2 is at: " << cDevice[1] << endl;

    openDev(cDevice[0], &(hdwfs[0]));
    openDev(cDevice[1], &(hdwfs[1]));
    cout << hdwfs.size() << endl;
    cout << "openDevs() returned [" << (hdwfs)[0] << ", " << (hdwfs)[1] << "]" << endl;


    if (hdwfs[0] != -1)
    {
        setupAnalogRead(hdwfs[0], 0, 5, 0);
        setupAnalogRead(hdwfs[0], 1, 5, 0);
    }
    if (hdwfs[1] != -1)
    {
        setupAnalogRead(hdwfs[1], 0, 5, 0);
        setupAnalogRead(hdwfs[1], 1, 5, 0);
    }
    // wait at least 2 seconds with Analog Discovery for the offset to stabilize, before the first reading after device open or offset/range change
    Wait(2);

    while(true)
    {

    //TODO: Add thread inits here

    //TODO: Join the data collection threads to wait for next set of data

    //TODO: Start Cross Correlation Thread after other threads have finished

    }
    // before application exit make sure to close all opened devices by this process
    FDwfDeviceCloseAll();


    //Check data for errors
    cout << datas1.size() << endl;

    for(unsigned int i = 0; i < datas1.size(); ++i)
    {
        if (datas1[i] < CENTER - DEVIATION || datas1[i] > CENTER + DEVIATION)
        {
            errs.push_back(datas1[i]);
        }
        if (datas2[i] < CENTER - DEVIATION || datas2[i] > CENTER + DEVIATION)
        {
            errs.push_back(datas2[i]);
        }
    }
    for (unsigned int i = 0; i < datas3.size(); ++i)
    {
        if (datas3[i] < CENTER - DEVIATION || datas3[i] > CENTER + DEVIATION)
        {
            errs.push_back(datas3[i]);
        }
        if (datas4[i] < CENTER - DEVIATION || datas4[i] > CENTER + DEVIATION)
        {
            errs.push_back(datas4[i]);
        }
    }

    cout << errs.size() << endl;
    cout << "Time Elapsed: " << end - start << endl;
    return 0;
}

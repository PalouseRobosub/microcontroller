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

    openDevs(cDevice[0], &(hdwfs[0]));
    openDevs(cDevice[1], &(hdwfs[1]));
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
    time_t start, end;
    start = time(NULL);
    for(int i = 0; i < 50000; i++){
        // fetch analog input information from the device
        if (hdwfs[0] != -1)
        {
            v11 = analogReadSingleDataDev(hdwfs[0], 0);
            v12 = analogReadSingleDataDev(hdwfs[0], 1);
            //printf("AD1:\n");
            //printf("\tCH1: %.3lf V\n", v11);
            //datas1.push_back(v11);
            //printf("\tCH2: %.3lf V\n", v12);
            //datas2.push_back(v12);
        }
        if (hdwfs[1] != -1)
        {
            v21 = analogReadSingleDataDev(hdwfs[1], 0);
            v22 = analogReadSingleDataDev(hdwfs[1], 1);
            //printf("AD2:\n");
            //printf("\tCH1: %.3lf V\n", v21);
            //datas3.push_back(v21);
            //printf("\tCH2: %.3lf V\n", v22);
            //datas4.push_back(v22);
        }


    }
    end = time(NULL);
    // before application exit make sure to close all opened devices by this process
    FDwfDeviceCloseAll();


    //Check data for errors
    cout << datas1.size() << endl;

    for(unsigned int i = 0; i < datas1.size(); ++i)
    {
        if (datas1[i] < 2.45 || datas1[i] > 2.55)
        {
            errs.push_back(datas1[i]);
        }
        if (datas2[i] < 2.45 || datas2[i] > 2.55)
        {
            errs.push_back(datas2[i]);
        }
    }
    for (unsigned int i = 0; i < datas3.size(); ++i)
    {
        if (datas3[i] < 2.45 || datas3[i] > 2.55)
        {
            errs.push_back(datas3[i]);
        }
        if (datas4[i] < 2.45 || datas4[i] > 2.55)
        {
            errs.push_back(datas4[i]);
        }
    }

    cout << errs.size() << endl;
    cout << "Time Elapsed: " << end - start << endl;
    return 0;
}
